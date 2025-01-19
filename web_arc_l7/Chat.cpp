#include "chat.hpp"

Chat::Chat() {}

void Chat::addUser(User* user) {
    users.push_back(user);
}

void Chat::addChannel(Channel* channel) {
    channels.push_back(channel);
}

void Chat::addUserToChannel(User* user, Channel* channel) {
    channel->addUser(user);
    user->addChannel(channel);
}

void Chat::removeUserFromChannel(User* user, Channel* channel) {
    channel->removeUser(user);
    user->removeChannel(channel);
}

void Chat::removeUser(User* user) {
    // Remove from all channels
    for (auto& channel : channels) {
        channel->removeUser(user);
    }
    // Remove from the user list
    auto it = std::find(users.begin(), users.end(), user);
    if (it != users.end()) {
        users.erase(it);
    }
}

void Chat::removeChannel(Channel* channel) {
    // Remove from all users
    for (auto& user : users) {
        user->removeChannel(channel);
    }
    // Remove from the channel list
    auto it = std::find(channels.begin(), channels.end(), channel);
    if (it != channels.end()) {
        channels.erase(it);
    }
}
