#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "fdio.h"


#define NB_FDS (2)
#define BUFFER_SIZE (4096)
static char buffer_s [BUFFER_SIZE];

static Buffer readPollResult(struct pollfd fds[NB_FDS]);
static Buffer readFrom(int const fd);

void fdio_send(int const destFd, Buffer const buf)
{
    int sent = 0;
    while (sent < buf.size)
    {
        int const chunkSize = write(destFd, buf.data + sent, buf.size - sent);
        sent += chunkSize;
    }
}

Buffer fdio_receive(int const socketFd, int const pipeFd)
{
    struct pollfd fds[] = {
        {
            .fd = socketFd,
            .events = POLLIN,
            .revents = 0,
        },
        {
            .fd = pipeFd,
            .events = POLLIN,
            .revents = 0,
        },
    };

    int const noTimeout = -1;
    int const pollResult = poll(fds, NB_FDS, noTimeout);
    if (pollResult == -1)
    {
        perror("Call to poll failed");
        exit(EXIT_FAILURE);
    }

    Buffer const result = readPollResult(fds);
    return result;
}

static Buffer readPollResult(struct pollfd fds[NB_FDS])
{
    for (int i = 0; i < NB_FDS; i++)
    {
        int const fd = fds[i].fd;
        short const events = fds[i].revents;
        if ((events & POLLERR) == POLLERR)
        {
            fprintf(stderr, "Poll returned POLLERR");
            exit(EXIT_FAILURE);
        }
        else if ((events & POLLHUP) == POLLHUP)
        {
            Buffer const empty = {
                .originFd = fd,
                .size = 0,
                .data = buffer_s,
            };
            return empty;
        }
        else if ((events & POLLNVAL) == POLLNVAL)
        {
            fprintf(stderr, "Poll returned POLLNVAL");
            exit(EXIT_FAILURE);
        }
        else if ((events & POLLIN) == POLLIN)
        {
            Buffer const result = readFrom(fd);
            return result;
        }
    }

    Buffer const empty = {
        .originFd = -1,
        .size = 0,
        .data = buffer_s,
    };
    return empty;
}

static Buffer readFrom(int const fd)
{
    ssize_t const nbRead = read(fd, buffer_s, BUFFER_SIZE-1);
    if (nbRead == -1)
    {
        perror("Read failed");
        exit(EXIT_FAILURE);
    }
    buffer_s[nbRead] = '\0';

    Buffer const result = {
        .originFd = fd,
        .size = nbRead,
        .data = buffer_s,
    };
    return result;
}
