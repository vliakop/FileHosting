#include <iostream>
#include "common/commons.h"
#include "common/ClientList.h"
#include "server_functions/server_fnc.h"
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {


    if (argc != 3) {
        custom_exit("Server:Invalid number of arguments");
    }

    int portNum;
    server_init(&portNum, argv);

    /* Set up the connection */
    int port, sock, newsock;

    unsigned int serverlen, clientlen;

    struct sockaddr_in server, client;
    struct sockaddr *serverptr, *clientptr;
    struct hostent *rem;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        custom_exit("Socket");
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(portNum);

    serverptr = (struct sockaddr *) &server;
    serverlen = sizeof(server);

    int s_bind, s_listen;

    s_bind = bind(sock, serverptr, serverlen);
    if(s_bind < 0) {
        custom_exit("Server bind");
    }

    s_listen = listen(sock, 5);
    if(s_listen < 0) {
        custom_exit("Server listen");
    }

    cout<<"I am server with PID "<<(int) getpid()<<" waiting for a request at port "<<port<<endl;

    clientptr = (struct sockaddr *) &client;
    clientlen = sizeof(client);
    ClientList *clientList = new ClientList();
    bool flag = true;
    while(flag == true) {
        cout<<"Server waiting"<<endl;
        newsock = accept(sock, clientptr, &clientlen);
        cout<<"Received"<<endl;
        if(newsock < 0) {
            custom_exit("Server accept");
        }
        getInstruction(newsock, clientList);
    }
    delete clientList;

}