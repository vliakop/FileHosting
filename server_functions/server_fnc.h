#pragma once

#include "../common/ClientList.h"


void server_init(int *portNum, char *argv[]);
void getInstruction(int fd, ClientList *clientList);
void parseInstruction(char *instruction, int fd, ClientList *clientList);
void log_on(long clientIP, short clientPort, ClientList *clientList, int fd);
void get_clients(int fd, ClientList *clientList);
void log_off(long clientIP, short clientPort, ClientList *clientList, int fd);

