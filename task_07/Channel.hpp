#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include <algorithm>
#include <string>
#include "user.hpp"
class User;
class Channel {
public:
    Channel(std::string name);
    void addUser(User* user);
    void removeUser(User* user);
    const std::string& getName() const;

private:
    std::string name;
    std::vector<User*> users;
};

#endif
