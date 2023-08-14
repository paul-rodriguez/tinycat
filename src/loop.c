#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "fdio.h"
#include "loop.h"

static int getResponseFd(Buffer const received, int const socketFd);

void loop(int const socketFd)
{
    bool reachedEof = false;
    while (!reachedEof)
    {
        Buffer const received = fdio_receive(socketFd, STDIN_FILENO);
        if (received.size > 0)
        {
            int const responseFd = getResponseFd(received, socketFd);
            fdio_send(responseFd, received);
        }
        else if (received.size == 0)
        {
            reachedEof = true;
        }
    }
}

static int getResponseFd(Buffer const received, int const socketFd)
{
    if (received.originFd == socketFd)
    {
        return STDOUT_FILENO;
    }
    else if (received.originFd == STDIN_FILENO)
    {
        return socketFd;
    }
    else
    {
        fprintf(stderr, "Internal tinycat error");
        exit(EXIT_FAILURE);
    }
}

