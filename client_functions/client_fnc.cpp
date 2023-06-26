#include <cstring>
#include <cstdlib>
#include <netdb.h>
#include <netinet/in.h>
#include <cstdio>
#include "../common/commons.h"
#include "../common/ClientList.h"
#include <arpa/inet.h>

void client_init(char dirName[], int *portNum, int *workerThreads, int *bufferSize, int *serverPort, char *serverIP, char *argv[]) {

    for (int i = 1; i < 13; i += 2) {
        if (strcmp(argv[i], "-d") == 0) {
            strncpy(dirName, argv[i+1], strlen(argv[i+1]));
        } else if (strcmp(argv[i], "-p") == 0) {
            *portNum = atoi(argv[i+1]);
        } else if (strcmp(argv[i], "-w") == 0) {
            *workerThreads = atoi(argv[i+1]);
        } else if (strcmp(argv[i], "-b") == 0) {
            *bufferSize = atoi(argv[i+1]);
        } else if (strcmp(argv[i], "-sp") == 0) {
            *serverPort = atoi(argv[i+1]);
        } else if (strcmp(argv[i], "-sip") == 0) {
            strcpy(serverIP, argv[i+1]);
        } else {
            custom_exit("Client init: invalid arguments");
        }
    }
}

void log_on(int fd, short port) {

    char ip[] = "127.0.0.1";
//    struct hostent *rem;
//    if (rem  == NULL) {
//        custom_exit("Client log_on gethostbyname");
//    }
    char buf[128];
    sprintf(buf, "LOG_ON %ld %d", htonl(inet_addr(ip)), htons(port));
    int instruction_length = strlen(buf);
    my_write(fd, &instruction_length, sizeof(int));
    my_write(fd, buf, instruction_length);
}

void get_clients(int fd, ClientList *clientList) {

    char buf[] = "GET_CLIENTS";
    int instruction_length = strlen(buf);
    my_write(fd, &instruction_length, sizeof(int));
    my_write(fd, buf, instruction_length);

    int response_length;
    my_read(fd, &response_length, sizeof(int));
    char *response = (char *) malloc(response_length);
    char *token;
    char delims[] = " \n";
    token = strtok(response, delims);
    isNull(token);
    token = strtok(NULL, delims);
    while(token != NULL) {
        long clientIP = atol(token);
        token = strtok(NULL, delims);
        isNull(token);
        short clientPort = atoi(token);
        clientList->add(clientIP, clientPort);
        token = strtok(NULL, delims);
    }
    free(response);
}
void log_off(int fd, short port) {

    char ip[] = "127.0.0.1";
//    struct hostent *rem;
//    if (rem  == NULL) {
//        custom_exit("Client log_on gethostbyname");
//    }
    char buf[128];
    sprintf(buf, "LOG_OFF %ld %d", htonl(inet_addr(ip)), htons(port));
    int instruction_length = strlen(buf);
    my_write(fd, &instruction_length, sizeof(int));
    my_write(fd, buf, instruction_length);
}