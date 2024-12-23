#include "players_info.hpp"
#include <sstream>
#include <iostream>

void PlayerInfo::addPlayerIP(std::string clientID, std::string ip) {

    clientMap[clientID] = ip;
}

std::string PlayerInfo::getPlayerIP(std::string clientID) {
    return clientMap[clientID];
}

std::string PlayerInfo::getAllPlayerIPs(void) {
    std::ostringstream buffer;
    for (auto& client : clientMap) {
        buffer << client.first<< ":"<< client.second << ",";
    }
    return buffer.str().substr(0, buffer.str().size() - 1);
}

std::map<std::string, std::string> PlayerInfo::getPlayerIPMap()
{
    return clientMap;
}

void PlayerInfo::updatePlayerIPs(std::string msg_str)
{
    std::istringstream iss(msg_str);
    std::string token;
    while(std::getline(iss, token, ',')) {
        size_t colonPos = token.find(':');
        std::string clientID = token.substr(0, colonPos);
        std::string ip = token.substr(colonPos + 1, token.size());
        clientMap[clientID] = ip;
    }
}
