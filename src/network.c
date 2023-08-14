#include <sys/socket.h>

#include "cleanup.h"
#include "network.h"

int network_acceptSocket(int const listenSocket)
{
    struct sockaddr clientAddr = {};
    socklen_t clientAddrLen = sizeof(clientAddr);
    int const resultSocket =
        accept(listenSocket, &clientAddr, &clientAddrLen);
    registerOpenedSocket(resultSocket);
    return resultSocket;
}
