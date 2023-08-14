#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "cleanup.h"
#include "config.h"
#include "unix.h"

static struct sockaddr_un unix_resolve(char const * const path);
static int unix_connectSocket(struct sockaddr const * addr);
static int unix_bind(struct sockaddr const * addr);
static void unix_listen(int const socket);

int unix_client(char const * const path)
{
    struct sockaddr_un const addr = unix_resolve(path);
    int const socket = unix_connectSocket((struct sockaddr const *)&addr);
    return socket;
}

int unix_listenSocket(char const * const path)
{
    setupCleanup();
    struct sockaddr_un const addr = unix_resolve(path);
    int const socket = unix_bind((struct sockaddr const *)&addr);
    registerCreatedUnixSocket(path);
    unix_listen(socket);
    return socket;
}

static int unix_bind(struct sockaddr const * addr)
{
    int const result = socket(AF_UNIX, SOCK_STREAM, 0);
    if (result == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (bind(result, addr, sizeof(struct sockaddr_un)) == -1)
    {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }
    return result;
}

static void unix_listen(int const socket)
{
    if (listen(socket, MAX_CONNECTIONS) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
}

static struct sockaddr_un unix_resolve(char const * const path)
{
    struct sockaddr_un result =
    {
        .sun_family = AF_UNIX,
        .sun_path = {},
    };
    strncpy(result.sun_path, path, MAX_SOCKET_PATH_LEN-1);
    return result;
}

static int unix_connectSocket(struct sockaddr const * addr)
{
    int const result = socket(AF_UNIX, SOCK_STREAM, 0);
    if (result == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int const connectErr = connect(
            result,
            addr,
            sizeof(struct sockaddr_un));
    if (connectErr != 0)
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    return result;
}
