#include <cstring>
#include <cstdlib>
#include "server_fnc.h"
#include "../common/commons.h"
#include "../common/ClientList.h"





void server_init(int *portNum, char *argv[]) {

    if (strcmp(argv[1], "-p") != 0) {
        custom_exit("Server init: No of arguments");
    }
    *portNum = atoi(argv[2]);
}

void getInstruction(int fd, ClientList *clientList) {

    int instruction_length;
    my_read(fd, &instruction_length, sizeof(int));
    if(instruction_length <= 0) {
        custom_exit("server getInstruction instruction length");
    }
    char *instruction = (char *)malloc(instruction_length + 1);
    my_read(fd, instruction, instruction_length);
    parseInstruction(instruction, fd, clientList);
    free(instruction);

}

void parseInstruction(char *instruction, int fd, ClientList *clientList) {

    char *instruction_copy = (char *) malloc(strlen(instruction) + 1);
    strcpy(instruction_copy, instruction);

    char delims[] = " \n";
    char *token;
    token = strtok(instruction, delims);
    isNull(token);
    if(strcmp(token, "LOG_ON") == 0) {
        token = strtok(NULL, delims);
        isNull(token);
        long clientIP = atol(token);
        token = strtok(NULL, delims);
        isNull(token);
        short clientPort = atoi(token);
        log_on(clientIP, clientPort, clientList, fd);
    } else if (strcmp(token, "GET_CLIENTS") == 0) {
        get_clients(fd, clientList);
    } else if (strcmp(token, "LOG_OFF") == 0) {
        token = strtok(NULL, delims);
        isNull(token);
        long clientIP = atol(token);
        token = strtok(NULL, delims);
        isNull(token);
        short clientPort = atoi(token);
        log_off(clientIP, clientPort, clientList, fd);
    } else {
        custom_exit("Server instruction parsing");
    }
    free(instruction_copy);
}
void log_on(long clientIP, short clientPort, ClientList *clientList, int fd) {

    bool found = clientList->contains(clientIP, clientPort);
    if (found == false) {
//        clientList->broadcast_on(clientIP, clientPort, fd);
        clientList->add(clientIP, clientPort);
    }
}

void get_clients(int fd, ClientList *clientList) {
    clientList->get_clients(fd);
}

void log_off(long clientIP, short clientPort, ClientList *clientList, int fd) {

    bool deactivation = clientList->deactivate(clientIP, clientPort);
    if(deactivation == true) {
//        clientList->broadcast_off(clientIP, clientPort, fd);
    } else {
        char response[] = "ERROR_IP_PORT_NOT_FOUND_IN_LIST";
        my_write(fd, response, strlen(response));
    }
}

