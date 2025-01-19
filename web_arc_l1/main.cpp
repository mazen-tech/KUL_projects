#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

const int BUFFER_SIZE = 4096;
const std::string HOST = "telehack.com";
const int PORT = 23;

void handle_telnet_negotiation(const std::vector<unsigned char>& data, int sockfd) {
    // Respond to Telnet options as needed (e.g., WONT/WILL for options not supported).
    // For simplicity, we'll respond WONT to all options in this implementation.
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] == 255) { // IAC
            if (i + 2 < data.size()) {
                unsigned char command = data[i + 1];
                unsigned char option = data[i + 2];
                if (command == 251 || command == 252) { // WILL or WONT
                    unsigned char response[] = {255, 252, option}; // IAC WONT <option>
                    send(sockfd, response, sizeof(response), 0);
                } else if (command == 253 || command == 254) { // DO or DONT
                    unsigned char response[] = {255, 254, option}; // IAC DONT <option>
                    send(sockfd, response, sizeof(response), 0);
                }
                i += 2; // Skip the command and option bytes
            }
        }
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "23.73.252.68", &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        close(sockfd);
        return 1;
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        return 1;
    }

    std::cout << "Connected to " << HOST << std::endl;

    char buffer[BUFFER_SIZE];
    std::vector<unsigned char> received_data;

    // Receive data from the server
    while (true) {
        ssize_t bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received < 0) {
            perror("Receive failed");
            break;
        } else if (bytes_received == 0) {
            std::cout << "Connection closed by server" << std::endl;
            break;
        }

        // Null-terminate and print the received data
        buffer[bytes_received] = '\0';
        received_data.insert(received_data.end(), buffer, buffer + bytes_received);

        // Handle Telnet negotiation commands
        handle_telnet_negotiation(received_data, sockfd);

        // Print non-Telnet data (for demonstration purposes)
        for (char c : received_data) {
            if (c >= 32 || c == '\n' || c == '\r') {
                std::cout << c;
            }
        }
        received_data.clear();
    }

    close(sockfd);
    return 0;
}
