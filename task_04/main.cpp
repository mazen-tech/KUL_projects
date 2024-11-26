// main.cpp
#include "Server.h"

int main() {
    std::cout << "Echo Server" << std::endl;

    Server server(8888, 30);
    server.run();

    return 0;
}
