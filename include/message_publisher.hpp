#pragma once
#include <zmq.hpp>
#include <iostream>
#include <string>
#include <thread>
#include "players_info.hpp"
#include "networking.hpp"
#include "game_objects.hpp"

using namespace zmq;
using namespace std;

extern bool isDisconnected;

class MessagePublisher
{
    private:
        context_t context_;
        socket_t publisher_;
        int port_;

        GameObjects& game_objects_;
        PlayerInfo& player_info_;

        thread publisher_thread_;
        void run();

    public:
        MessagePublisher(int port, GameObjects& game_objects, PlayerInfo& player_info);
        ~MessagePublisher();
};