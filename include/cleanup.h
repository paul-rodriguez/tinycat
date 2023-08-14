#pragma once

void setupCleanup();

void registerCreatedUnixSocket(char const * const path);
void registerOpenedSocket(int const socket);
void closeSocket(int const socket);

void handleInterrupt();
