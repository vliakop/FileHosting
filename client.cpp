#include <iostream>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <unistd.h>
#include <cstring>
#include "common/commons.h"
#include "client_functions/client_fnc.h"
#include "server_functions/server_fnc.h"

using namespace std;


int main(int argc, char *argv[]) {

    char dirName[128];
    int portNum, workerThreads, bufferSize, serverPort;
    char serverIP[33];
//    char serverIP[] = "localhost";

    if(argc != 13) {
        custom_exit("Client: invalid number of args");
    }
    client_init(dirName, &portNum, &workerThreads, &bufferSize, &serverPort, serverIP, argv);


    /* Set up the connection */

    int port, sock, option, s_connect;

    unsigned int serverlen;

    struct sockaddr_in server;
    struct sockaddr *serverptr;
    struct hostent *rem;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        custom_exit("Client socket");
    }

    server.sin_family = AF_INET;

//    bcopy((char *) rem->h_addr, (char *) &server.sin_addr, rem->h_length);
    rem = gethostbyname(serverIP);
    if (rem  == NULL) {
        custom_exit("Client gethostbyname");
    }
    memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
    cout<<"address is "<<server.sin_addr.s_addr<<endl;
    server.sin_port = htons(serverPort);
    serverptr = (struct sockaddr *) &server;
    serverlen = sizeof(server);

    s_connect = connect(sock, serverptr, serverlen);
    if (s_connect < 0) {
        custom_exit("Client connect");
    }
    ClientList *clientList = new ClientList();

    log_on(s_connect, port);
    get_clients(s_connect, clientList);
    log_off(s_connect, port);


}

