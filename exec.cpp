#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <string>

using namespace std;

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

int main() {
	cout << exec("ls");
	system("read -p 'Press Enter to continue...' var");
}