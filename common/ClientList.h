#pragma once


class ClientList {

public:

    struct ClientNode {
        long clientIP;
        short clientPort;
        bool active;
        ClientNode *next;

        ClientNode(long IP, short port, bool active = 1, ClientNode *next = NULL);
        ~ClientNode();

        long getClientIP();
        short getClientPort();
        bool getActive();
        void print();
    };

    ClientList();
    ~ClientList();

    void add(long IP, short port);
    bool contains(long IP, short port);
    bool deactivate(long IP, short port);
    int getSize();
    void print();

    // broadcasts
    void broadcast_on(long clientIP, short clientPort, int fd);
    void get_clients(int fd);
    void broadcast_off(long clientIP, short clientPort, int fd);

private:
    ClientNode *head;
    int size;
};

