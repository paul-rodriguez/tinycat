#include <stdio.h>
#include <stdlib.h>

#include "cleanup.h"
#include "ip.h"
#include "loop.h"
#include "network.h"
#include "server.h"
#include "unix.h"

static int listenSocket(
        SocketType const socketType,
        char const * const port,
        char const * const unixSocket);

void server(Args const * const args)
{
    SocketType const socketType = getSocketType(args);
    int const listenSock =
        listenSocket(socketType, args->port, args->unixSocket);

    do
    {
        int const socketFd = network_acceptSocket(listenSock);
        loop(socketFd);
        closeSocket(socketFd);
    } while(args->loop);
}

static int listenSocket(
        SocketType const socketType,
        char const * const port,
        char const * const unixSocket)
{
    int result;
    if (socketType == TcpSocket)
    {
        result = ip_listenSocket(port);
    }
    else
    {
        result = unix_listenSocket(unixSocket);
    }
    return result;
}
