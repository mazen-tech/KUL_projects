#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>
#include "channel.hpp"
#include <algorithm>

class Channel;
class User {
public:
    User(std::string nickname);
    void addChannel(Channel* channel);
    void removeChannel(Channel* channel);
    const std::string& getNickname() const;

private:
    std::string nickname;
    std::vector<Channel*> channels;
};

#endif
