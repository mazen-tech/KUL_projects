#include "user.hpp"

User::User(std::string nickname) : nickname(nickname) {}

void User::addChannel(Channel* channel) {
    channels.push_back(channel);
}

void User::removeChannel(Channel* channel) {
    auto it = std::find(channels.begin(), channels.end(), channel);
    if (it != channels.end()) {
        channels.erase(it);
    }
}

const std::string& User::getNickname() const {
    return nickname;
}
