#include <stdio.h>
#include <stdlib.h>

#include "cleanup.h"
#include "client.h"
#include "ip.h"
#include "loop.h"
#include "unix.h"

static int connectFileDescriptor(
        SocketType const socketType,
        char const * const address,
        char const * const port,
        char const * const unixSocket);

void client(Args const * const args)
{
    setupCleanup();
    SocketType const socketType = getSocketType(args);
    int const socketFd = connectFileDescriptor(
            socketType,
            args->address,
            args->port,
            args->unixSocket);
    registerOpenedSocket(socketFd);

    loop(socketFd);
}

static int connectFileDescriptor(
        SocketType const socketType,
        char const * const address,
        char const * const port,
        char const * const unixSocket)
{
    int result;
    if (socketType == TcpSocket)
    {
        result = ip_client(address, port);
    }
    else
    {
        result = unix_client(unixSocket);
    }
    return result;
}

