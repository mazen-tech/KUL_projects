#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Failed to connect to server\n";
        return 1;
    }

    const char* command = "ls";  // Command to send to the server
    send(sockfd, command, strlen(command), 0);

    char buffer[1024];
    int n = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (n < 0) {
        std::cerr << "Error reading from socket\n";
        return 1;
    }

    buffer[n] = '\0';  // Null-terminate the received string
    std::cout << "Server response: \n" << buffer << std::endl;

    close(sockfd);  // Close the connection
    return 0;
}
