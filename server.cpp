#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include "exec.h"

void handleClient(int clientSocket) {
    char buffer[256];
    
    // Receive command from client
    int n = read(clientSocket, buffer, 255);
    if (n < 0) {
        std::cerr << "Error reading from socket\n";
        return;
    }

    // Null-terminate the received string
    buffer[n] = '\0';
    std::cout << "Command received: " << buffer << std::endl;

    // Execute the command and get the output
    std::string result = exec(buffer);

    // Send back the result to the client
    n = write(clientSocket, result.c_str(), result.length());
    if (n < 0) {
        std::cerr << "Error writing to socket\n";
        return;
    }

    close(clientSocket);  // Close the connection
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(12345);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Failed to bind socket\n";
        return 1;
    }

    listen(serverSocket, 5);
    std::cout << "Server is listening on port 12345...\n";

    while (true) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) {
            std::cerr << "Failed to accept connection\n";
            continue;
        }

        std::thread clientThread(handleClient, clientSocket);
        clientThread.detach();  // Handle client in a new thread
    }

    close(serverSocket);
    return 0;
}
