#pragma once

/** Portable limit for the length of a unix socket path. */
#define MAX_SOCKET_PATH_LEN (108)

int unix_client(char const * const path);
int unix_listenSocket(char const * const path);
