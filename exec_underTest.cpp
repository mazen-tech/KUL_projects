#ifndef EXEC_H
#define EXEC_H

#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <thread>
#include <unistd.h>
#include <arpa/inet.h>

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define RESET   "\033[0m" 


using namespace std;

std::string exec(const char* cmd);
string exec(const char* cmd) {
	char buffer[128];
	string result = "";
	FILE* pipe = popen(cmd, "r");
	if (!pipe) 
		throw runtime_error("popen(): failed!");

	try {
		while (!feof(pipe)) {
			if (fgets(buffer, 128, pipe) != NULL)
				result += buffer;
		}
	}
	catch (...) {
		pclose(pipe);
		throw;
	}
	pclose(pipe);
	return result;
}

//function to handle each client
void handle_client(int client_sock)
{
	char buffer[1024];
	while (true)
	{
		memset(buffer, 0, sizeof(buffer));

		int bytes_rec = read(client_sock, buffer, sizeof(buffer) - 1);
		if (bytes_rec <= 0)
		{
			cout << RED << "Client disconnected \n" << RESET;
			cout << RED <<"READ: ended\n" << RESET;
			close(client_sock); //closing the ckint file descreptor
			return;
		}

		string command(buffer);
		command.erase(command.find_last_not_of("\r\n") + 1);

		if (command == "exit")
		{
			cout << "client requested to disconnect\n";
			close(client_sock);
			return;
		}

		try
		{
			string res = exec(command.c_str());
			write(client_sock, res.c_str(), res.size());
		}
		catch(const exception& e)
		{
			string errro_msg = "Error: " + string(e.what()) + "\n";
			write(client_sock, errro_msg.c_str(), errro_msg.size());
		}
	}
}

int main() {
	//cout << exec("ls");
	//system("read -p 'Press Enter to continue...' var");

	const int port = 8080;
	int server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock == -1)
	{
		cerr << RED << "function socket: failed\n" << RESET;
		return 1;
	}

	sockaddr_in server_address{};
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(port);

	if (listen(server_sock, 5) == -1)
	{
		cerr << RED << "Listing to port " << RESET << port << RED << "faild\n" << RESET;
		return 1;
	}

	cout << GREEN << "Server is running on port " << RESET <<  port << GREEN <<"...\n" << RESET;

    vector<thread> client_threads;

    while (true) {
        sockaddr_in client_address{};
        socklen_t client_len = sizeof(client_address);
        int client_socket = accept(server_sock, (sockaddr*)&client_address, &client_len);
        if (client_socket == -1) {
            cerr << RED << "Failed to accept connection.\n" << RESET;
            continue;
        }

        cout << "New client connected.\n";

        // Handle client in a separate thread
        client_threads.emplace_back(handle_client, client_socket);
    }

    close(server_sock);
    return 0;
}

#endif