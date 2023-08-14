#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "cleanup.h"
#include "config.h"
#include "ip.h"
#include "unix.h"

#define MAX_SOCKETS (MAX_CONNECTIONS + 1)
#define NO_SOCKET (-1)

static int openSockets_s[MAX_SOCKETS] = {};
static int nbSockets_s = 0;
static char unixSocketPath_s[MAX_SOCKET_PATH_LEN] = {};

void setupCleanup()
{
    signal(SIGINT, handleInterrupt);
}

void registerCreatedUnixSocket(char const * const path)
{
    strncpy(unixSocketPath_s, path, MAX_SOCKET_PATH_LEN);
}

void registerOpenedSocket(int const socket)
{
    if (nbSockets_s == MAX_SOCKETS)
    {
        fprintf(stderr,
                "Failed to register socket %d for cleanup: too many (%d) "
                "sockets already opened",
                socket,
                nbSockets_s);
    }
    openSockets_s[nbSockets_s] = socket;
    nbSockets_s++;
}

void closeSocket(int const socket)
{
    close(socket);
    for (int i = 0; i < nbSockets_s; i++)
    {
        int * const socketPtr = openSockets_s + i;
        if (*socketPtr == socket)
        {
            memmove(socketPtr, socketPtr + 1, nbSockets_s - 1 - i);
            openSockets_s[nbSockets_s-1] = NO_SOCKET;
            nbSockets_s--;
            return;
        }
    }
    fprintf(stderr,
            "Tried to unregister socket %d which wasn't in the cleanup list",
            socket);
    exit(EXIT_FAILURE);
}

void handleInterrupt()
{
    fprintf(stderr, "Received INT signal, closing sockets\n");
    for (int i = 0; i < nbSockets_s; i++)
    {
        int const shutdownErr = shutdown(openSockets_s[i], SHUT_RDWR);
        if (shutdownErr != 0)
        {
            perror("shutdown failed");
        }
        int const closeErr = close(openSockets_s[i]);
        if (closeErr != 0)
        {
            perror("close failed");
        }
        fprintf(stderr, "Closed socket %d\n", openSockets_s[i]);
    }

    if (unixSocketPath_s[0] != '\0')
    {
        int const unlinkErr = unlink(unixSocketPath_s);
        if (unlinkErr != 0)
        {
            perror("unlink failed");
        }
    }
    exit(0);
}
