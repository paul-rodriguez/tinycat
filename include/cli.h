#pragma once

#include <stdbool.h>

typedef enum
{
    TcpSocket,
    UnixSocket,
} SocketType;

typedef struct
{
    bool loop;
    bool listen;
    char const * unixSocket;
    char const * address;
    char const * port;
} Args;

Args parse(int argc, char const * * argv);

int toString(char * buf, int size, Args const * args);

SocketType getSocketType(Args const * args);
