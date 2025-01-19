#ifndef CHAT_HPP
#define CHAT_HPP

#include <vector>
#include "channel.hpp"
#include "user.hpp"
#include <algorithm>
class Chat {
public:
    Chat();
    void addUser(User* user);
    void addChannel(Channel* channel);
    void addUserToChannel(User* user, Channel* channel);
    void removeUserFromChannel(User* user, Channel* channel);
    void removeUser(User* user);
    void removeChannel(Channel* channel);

private:
    std::vector<User*> users;
    std::vector<Channel*> channels;
};

#endif
