#pragma once

#include "../common/ClientList.h"

void client_init(char dirName[], int *portNum, int *workerThreads, int *bufferSize, int *serverPort, char *serverIP, char *argv[]);
void log_on(int fd, short port);
void get_clients(int fd, ClientList *clientList);
void log_off(int fd, short port);

