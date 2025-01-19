// Server.h
#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#define RED     "\033[31m"  // Red color
#define GREEN   "\033[32m"  // Green color
#define RESET   "\033[0m"   // Reset color to default

class Server {
public:
    Server(int port, int max_clients);
    ~Server();

    void run();
    
private:
    int server_fd;
    int *client_socket;
    struct sockaddr_in server, address;
    int port;
    int max_clients;

    void initServer();
    void acceptClients();
    void handleConnections();
    void echoMessage(int client_sock);
};

#endif
