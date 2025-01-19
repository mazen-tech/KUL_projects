// Server.cpp
#include "Server.h"

Server::Server(int port, int max_clients) : port(port), max_clients(max_clients) {
    client_socket = new int[max_clients];
    for (int i = 0; i < max_clients; i++) {
        client_socket[i] = 0;
    }
}

Server::~Server() {
    delete[] client_socket;
}

void Server::initServer() {
    std::cout << "Creating socket... ";
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(1);
    }
    std::cout << "done" << std::endl;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    std::cout << "Binding socket... ";
    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Bind failed");
        exit(1);
    }
    std::cout << "done" << std::endl;
}

void Server::acceptClients() {
    std::cout << "Starting to listen... ";
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(1);
    }
    std::cout << "done" << std::endl;
}

void Server::handleConnections() {
    int addrlen = sizeof(struct sockaddr_in);
    fd_set readfds;
    char buffer[1501];
    int max_sd = server_fd;
    int cs;

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);

        for (int i = 0; i < max_clients; i++) {
            cs = client_socket[i];
            if (cs > 0) {
                FD_SET(cs, &readfds);
            }
            if (cs > max_sd) {
                max_sd = cs;
            }
        }

        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("Select error");
            break;
        }

        if (FD_ISSET(server_fd, &readfds)) {
            int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
            if (new_socket < 0) {
                perror("Accept failed");
                exit(1);
            }

            std::cout <<GREEN<< "New client connected: " << RESET <<inet_ntoa(address.sin_addr) << std::endl;

            for (int i = 0; i < max_clients; i++) {
                if (client_socket[i] == 0) {
                    client_socket[i] = new_socket;
                    break;
                }
            }
        }

        for (int i = 0; i < max_clients; i++) {
            cs = client_socket[i];
            if (FD_ISSET(cs, &readfds)) {
                int valread = recv(cs, buffer, 1500, 0);
                if (valread == 0) {
                    close(cs);
                    client_socket[i] = 0;
                    std::cout << RED << "Client disconnected" << RESET<<std::endl;
                } else if (valread > 0) {
                    buffer[valread] = '\0';
                    std::cout << GREEN << "Received message: " << RESET <<buffer << std::endl;
                    send(cs, buffer, valread, 0); // Echo the message back
                    std::cout << GREEN << "Echoed message back to client." << RESET << std::endl;
                }
            }
        }
    }
}

void Server::run() {
    initServer();
    acceptClients();
    handleConnections();
}
