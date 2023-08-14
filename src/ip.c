#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "cleanup.h"
#include "config.h"
#include "ip.h"

static struct addrinfo * serverAddrInfo(char const * const port);
static struct addrinfo * resolve(
        char const * const address,
        char const * const port);
static int socketConnect(struct addrinfo * const serverList);
static int socketListen(struct addrinfo * const addrList);

int ip_client(char const * const address, char const * const port)
{
    struct addrinfo * const serverList = resolve(address, port);
    int const socket = socketConnect(serverList);
    freeaddrinfo(serverList);

    return socket;
}

int ip_listenSocket(char const * const port)
{
    setupCleanup();
    struct addrinfo * const addrList = serverAddrInfo(port);
    int const socket = socketListen(addrList);
    freeaddrinfo(addrList);
    registerOpenedSocket(socket);
    return socket;
}

static int socketListen(struct addrinfo * const addrList)
{
    struct addrinfo * p;
    for (p = addrList; p != NULL; p = p->ai_next)
    {
        int const socketResult = socket(
                p->ai_family,
                p->ai_socktype,
                p->ai_protocol);
        if (socketResult == -1)
        {
            continue;
        }

        if (bind(socketResult, p->ai_addr, p->ai_addrlen) == 0)
        {
            listen(socketResult, MAX_CONNECTIONS);
            return socketResult;
        }

        close(socketResult);
    }
    if (p == NULL)
    {
        fprintf(stderr, "Binding failed.");
        exit(EXIT_FAILURE);
    }
    return -1;
}

static int socketConnect(struct addrinfo * const serverList)
{
    struct addrinfo * p;
    for (p = serverList; p != NULL; p = p->ai_next)
    {
        int const socketResult =
            socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (socketResult == -1)
        {
            continue;
        }

        int const connectErr =
            connect(socketResult, p->ai_addr, p->ai_addrlen);
        if (connectErr == 0)
        {
            return socketResult;
        }
        close(socketResult);
    }
    if (p == NULL)
    {
        fprintf(stderr, "Connection failed.");
        exit(EXIT_FAILURE);
    }
    return -1;
}

static struct addrinfo * serverAddrInfo(char const * const port)
{
    struct addrinfo const hints =
    {
        .ai_flags = AI_PASSIVE,
        .ai_family = AF_INET6,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = 0,
        .ai_addrlen = 0,
        .ai_addr = NULL,
        .ai_canonname = NULL,
        .ai_next = NULL,
    };

    struct addrinfo * result;
    int const getAddrInfoErr =
        getaddrinfo(NULL, port, &hints, &result);
    if (getAddrInfoErr != 0)
    {
        fprintf(stderr, "getaddrinfo failure: %d", getAddrInfoErr);
        exit(EXIT_FAILURE);
    }
    return result;
}

static struct addrinfo * resolve(
        char const * const address,
        char const * const port)
{
    struct addrinfo const hints =
    {
        .ai_flags = 0,
        /* Allow both IPv4 and IPv6 */
        .ai_family = AF_UNSPEC,
        /* TCP socket */
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = 0,
        .ai_addrlen = 0,
        .ai_addr = NULL,
        .ai_canonname = NULL,
        .ai_next = NULL,
    };

    struct addrinfo * result;
    int const getAddrInfoErr = getaddrinfo(
            address,
            port,
            &hints,
            &result);
    if (getAddrInfoErr != 0)
    {
        fprintf(stderr, "getaddrinfo failed: %d", getAddrInfoErr);
        exit(EXIT_FAILURE);
    }
    return result;
}

