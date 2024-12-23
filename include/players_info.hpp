#pragma once
#include <map>
#include <string>
class PlayerInfo
{
    private:
    std::map<std::string, std::string> clientMap; 

    public:
    void addPlayerIP(std::string clientID, std::string ip);
    std::string getPlayerIP(std::string clientID);
    std::string getAllPlayerIPs(void);
    std::map<std::string, std::string> getPlayerIPMap();
    void updatePlayerIPs(std::string msg_str);
};