#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <cstring>
#include "rlutil.h"
#include "iac.h" // For Telnet command decoding

using namespace std;

// Emulate getch() on Linux
int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

// Emulate kbhit() on Linux
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

int main() {
    int sock;
    struct sockaddr_in server;
    char buffer[1000];
    const char *hostname = "telehack.com";

    // Resolve hostname to IP address
    struct hostent *he = gethostbyname(hostname);
    if (he == NULL) {
        cerr << "Cannot resolve hostname" << endl;
        return 1;
    }

    struct in_addr **addr_list = (struct in_addr **)he->h_addr_list;
    char *ip = inet_ntoa(*addr_list[0]);
    cout << hostname << " has IP address: " << ip << endl;

    // Set up socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Socket creation error" << endl;
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(23);
    inet_pton(AF_INET, ip, &server.sin_addr);

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        cerr << "Connection error" << endl;
        return 1;
    }

    // Set non-blocking mode for the socket
    fcntl(sock, F_SETFL, O_NONBLOCK);

    // Main loop
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int inDataLength = recv(sock, buffer, sizeof(buffer), 0);

        if (inDataLength > 0) {
            decodeIAC((unsigned char*)buffer, inDataLength);
            cout << buffer;
        }

        if (kbhit()) {
            char ch = getch();
            send(sock, &ch, 1, 0);

            // Handle specific key inputs
            if (ch == 'c') {
                rlutil::cls(); // Clear screen
            } else if (ch == 'u') {
                rlutil::locate(1, 1); // Move cursor to top left
            }
        }

        usleep(10000); // 10 ms sleep to reduce CPU usage
    }

    close(sock);
    return 0;
}