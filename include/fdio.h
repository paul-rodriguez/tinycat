#pragma once

typedef struct {
    int originFd;
    int size;
    char const * data;
} Buffer;

void fdio_send(int const destFd, Buffer const buf);

Buffer fdio_receive(int const socketFd, int const pipeFd);

