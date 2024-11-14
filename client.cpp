#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

int main() {
    // Create a client socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        cerr << "Socket creation failed!" << endl;
        return -1;
    }

    // Configure server address
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);  // Server port
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);  // Server address (localhost)

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        cerr << "Connection to server failed!" << endl;
        close(clientSocket);
        return -1;
    }

    cout << "Connected to the server!" << endl;

    // Communicate with the server
    string command;
    char buffer[4096];
    while (true) {
        cout << "Enter a command to execute (or 'exit' to quit): ";
        getline(cin, command);

        if (command == "exit") {
            cout << "Exiting..." << endl;
            break;
        }

        // Send the command to the server
        send(clientSocket, command.c_str(), command.size(), 0);

        // Receive the response
        ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';  // Null-terminate the string
            cout << "Server response:\n" << buffer << endl;
        } else {
            cout << "Error receiving data or server closed the connection." << endl;
            break;
        }
    }

    close(clientSocket);
    return 0;
}

