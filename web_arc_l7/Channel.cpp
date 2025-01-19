#include "channel.hpp"

Channel::Channel(std::string name) : name(name) {}

void Channel::addUser(User* user) {
    users.push_back(user);
}

void Channel::removeUser(User* user) {
    auto it = std::find(users.begin(), users.end(), user);
    if (it != users.end()) {
        users.erase(it);
    }
}

const std::string& Channel::getName() const {
    return name;
}
