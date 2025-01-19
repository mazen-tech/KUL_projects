#include <iostream>
#include <string>
#include <list>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

using namespace std;


list<string> tokenizer(string text, char delimiter = ' ') {
    list<string> result;
    string token;
    istringstream input(text);

    while (getline(input, token, delimiter)) {
        if (!token.empty()) { // Skip empty tokens
            result.push_back(token);
        }
    }

    return result;
}

class Chat {
private:
    unordered_map<string, string> users; // Maps nicknames to user data
    unordered_map<string, unordered_set<string>> channels; // Maps channel names to sets of nicknames

public:
    void setNickname(const string& nickname) {
        if (users.find(nickname) == users.end()) {
            users[nickname] = "";
            cout << "Nickname set: " << nickname << endl;
        } else {
            cout << "Error: Nickname already in use." << endl;
        }
    }

    void listUsers() {
        cout << "Active users:" << endl;
        for (const auto& [nickname, _] : users) {
            cout << "- " << nickname << endl;
        }
    }

    void createChannel(const string& channelName) {
        if (channels.find(channelName) == channels.end()) {
            channels[channelName] = {};
            cout << "Channel created: " << channelName << endl;
        } else {
            cout << "Error: Channel already exists." << endl;
        }
    }

    void joinChannel(const string& channelName, const string& nickname) {
        if (channels.find(channelName) != channels.end()) {
            channels[channelName].insert(nickname);
            cout << nickname << " joined channel: " << channelName << endl;
        } else {
            cout << "Error: Channel does not exist." << endl;
        }
    }

    void leaveChannel(const string& channelName, const string& nickname) {
        if (channels.find(channelName) != channels.end() && channels[channelName].count(nickname)) {
            channels[channelName].erase(nickname);
            cout << nickname << " left channel: " << channelName << endl;
        } else {
            cout << "Error: Channel or user not found." << endl;
        }
    }

    void removeChannel(const string& channelName) {
        if (channels.find(channelName) != channels.end()) {
            channels.erase(channelName);
            cout << "Channel removed: " << channelName << endl;
        } else {
            cout << "Error: Channel does not exist." << endl;
        }
    }

    void sendToChannel(const string& channelName, const string& message) {
        if (channels.find(channelName) != channels.end()) {
            cout << "Message to channel " << channelName << ": " << message << endl;
        } else {
            cout << "Error: Channel does not exist." << endl;
        }
    }

    void sendToUser(const string& nickname, const string& message) {
        if (users.find(nickname) != users.end()) {
            cout << "Message to user " << nickname << ": " << message << endl;
        } else {
            cout << "Error: User does not exist." << endl;
        }
    }

    void parseCommand(const string& command) {
        list<string> tokens = tokenizer(command, '$');
    if (tokens.empty()) return;

    string operation = tokens.front();
    tokens.pop_front();

    // Trim whitespace from operation
    operation.erase(0, operation.find_first_not_of(" \t"));
    operation.erase(operation.find_last_not_of(" \t") + 1);

    if (operation == "SETNICKNAME" && !tokens.empty()) {
        setNickname(tokens.front());
    } else if (operation == "LISTUSERS") {
        listUsers();
    } else if (operation == "CREATECHANNEL" && !tokens.empty()) {
        createChannel(tokens.front());
    } else if (operation == "JOINCHANNEL" && !tokens.empty()) {
        joinChannel(tokens.front(), "user"); // Replace "user" with actual nickname logic
    } else if (operation == "LEAVECHANNEL" && !tokens.empty()) {
        leaveChannel(tokens.front(), "user"); // Replace "user" with actual nickname logic
    } else if (operation == "REMOVECHANNEL" && !tokens.empty()) {
        removeChannel(tokens.front());
    } else if (operation == "SENDTOCHANNEL" && tokens.size() >= 2) {
        string channelName = tokens.front();
        tokens.pop_front();
        sendToChannel(channelName, tokens.front());
    } else if (operation == "SENDTOUSER" && tokens.size() >= 2) {
        string nickname = tokens.front();
        tokens.pop_front();
        sendToUser(nickname, tokens.front());
    } else {
        cout << "Error: Invalid command or arguments." << endl;
        }
    }
};

int main() {
    Chat chat;
    string command;

    // Example usage
    command = "SETNICKNAME $JohnDoe$";
    chat.parseCommand(command);

    command = "CREATECHANNEL $General$";
    chat.parseCommand(command);

    command = "JOINCHANNEL $General$";
    chat.parseCommand(command);

    command = "SENDTOCHANNEL $General$ Hello, everyone!";
    chat.parseCommand(command);

    command = "SENDTOUSER $JohnDoe$ Private message content";
    chat.parseCommand(command);

    command = "LISTUSERS";
    chat.parseCommand(command);

    return 0;
}
