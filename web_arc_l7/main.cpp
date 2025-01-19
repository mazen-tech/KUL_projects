#include <iostream>
#include "chat.hpp"
#include "user.hpp"
#include "channel.hpp"

int main() {
    // Create a chat server
    Chat chatServer;

    // Create users
    User* user1 = new User("Alice");
    User* user2 = new User("Bob");

    // Add users to chat server
    chatServer.addUser(user1);
    chatServer.addUser(user2);

    // Create channels
    Channel* channel1 = new Channel("General");
    Channel* channel2 = new Channel("Tech");

    // Add channels to chat server
    chatServer.addChannel(channel1);
    chatServer.addChannel(channel2);

    // Add users to channels
    chatServer.addUserToChannel(user1, channel1);
    chatServer.addUserToChannel(user2, channel2);

    // Show user and channel info
    std::cout << user1->getNickname() << " is in channel: " << channel1->getName() << std::endl;
    std::cout << user2->getNickname() << " is in channel: " << channel2->getName() << std::endl;

    // Clean up memory
    delete user1;
    delete user2;
    delete channel1;
    delete channel2;

    return 0;
}
