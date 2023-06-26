#include <iostream>
#include <netdb.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "ClientList.h"
#include "commons.h"

using namespace std;

ClientList::ClientNode::ClientNode(long IP, short port, bool active, ClientList::ClientNode *next) {

    this->clientIP = IP;
    this->clientPort = port;
    this->active = active;
    this->next = next;
}

ClientList::ClientNode::~ClientNode() {}

long ClientList::ClientNode::getClientIP() {

    return this->clientIP;
}

short ClientList::ClientNode::getClientPort() {

    return this->clientPort;
}

bool ClientList::ClientNode::getActive() {

    return this->active;
}

void ClientList::ClientNode::print() {

    cout<<"Node with IP: "<<this->clientIP<<" and port: "<<this->getClientPort()<<endl;
}

ClientList::ClientList() {

    size = 0;
    head = NULL;
}

ClientList::~ClientList() {

    ClientNode *n = head;
    while(n != NULL){
        head = head->next;
        delete n;
        n = head;
        size--;
    }
}

void ClientList::add(long IP, short port) {

    // TODO if exist check
    ClientNode *n = new ClientNode(IP, port, true, head);
    head = n;
    size += 1;
}


bool ClientList::contains(long IP, short port) {

    if (size == 0) {
        return false;
    }
    ClientNode *n = head;
    while(n != NULL) {
        if (n->clientIP == IP && n->clientPort == port) {
            return true;
        }
        n = n->next;
    }
    return false;
}

bool ClientList::deactivate(long IP, short port) {

    if (size == 0) {
        return false;
    }

    if (contains(IP, port) == false) {
        return false;
    }

    ClientNode *n = head;
    while(n != NULL) {
        if(n->clientIP == IP && n->clientPort == port) {
            n->active = false;
            return true;
        }
        n = n->next;
    }
    return false;
}

int ClientList::getSize() {
    return this->size;
}

void ClientList::print() {

    ClientNode *n = head;
    while(n != NULL) {
        n->print();
        n = n->next;
    }
}

// Broadcasts

void ClientList::broadcast_on(long clientIP, short clientPort, int fd) {

    int port, sock, option, s_connect;

    unsigned int serverlen;

    struct sockaddr_in server;
    struct sockaddr *serverptr;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        custom_exit("Broadcast_on socket");
    }
    server.sin_family = AF_INET;

    char buf[32];
    sprintf(buf, "USER_ON %ld %d\n", clientIP, clientPort);
    ClientNode *n = head;
    while(n != NULL) {
        if(n->active == true) {
            server.sin_addr.s_addr = n->clientIP;
            server.sin_port = n->clientPort;
            serverptr = (struct sockaddr *) &server;
            serverlen = sizeof(server);

            s_connect = connect(sock, serverptr, serverlen);
            if(s_connect < 0) {
                custom_exit("Broadcast_on s_connect");
            }
            my_write(fd, buf, strlen(buf));
            close(s_connect); // TODO remove?
        }
    }
}

void ClientList::get_clients(int fd) {

    char *msg = (char *) malloc(13); // CLIENT_LIST\0
    strcpy(msg, "CLIENT_LIST");

    ClientNode *n = head;
    while(n != NULL) {
        if (n->active == true) {
            char buf[128];
            memset(buf, 0, 128);
            sprintf(buf, " %ld %d", n->clientIP, n->clientPort);
            msg = (char *) realloc(msg, strlen(msg) + strlen(buf) + 1);
            strncat(msg, buf, strlen(buf));
        }
        n = n->next;
    }
    int instruction_length = strlen(msg);
    my_write(fd, &instruction_length, sizeof(int));
    my_write(fd, msg, instruction_length);
    free(msg);
}

void ClientList::broadcast_off(long clientIP, short clientPort, int fd) {

    int port, sock, option, s_connect;

    unsigned int serverlen;

    struct sockaddr_in server;
    struct sockaddr *serverptr;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        custom_exit("Broadcast_off socket");
    }
    server.sin_family = AF_INET;

    char buf[32];
    sprintf(buf, "USER_OFF %ld %d\n", clientIP, clientPort);
    ClientNode *n = head;
    while(n != NULL) {
        if(n->active == true) {
            server.sin_addr.s_addr = n->clientIP;
            server.sin_port = n->clientPort;
            serverptr = (struct sockaddr *) &server;
            serverlen = sizeof(server);

            s_connect = connect(sock, serverptr, serverlen);
            if(s_connect < 0) {
                custom_exit("Broadcast_off s_connect");
            }
            my_write(fd, buf, strlen(buf));
            close(s_connect); // TODO remove?
        }
    }
}