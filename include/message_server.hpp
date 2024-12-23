#pragma once
#include <zmq.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "timeline.hpp"
#include "game_objects.hpp"
#include "players_info.hpp"
#include "event.hpp"
#include "event_manager.hpp"

using namespace zmq;
using namespace std;

extern Timeline* global_time;

class MessageServer 
{
    public:
        MessageServer(int port, GameObjects& game_objects, PlayerInfo& player_info);
        ~MessageServer();
        
    private:
        context_t context_;
        socket_t socket_;
        int port_;
        int client_count_;

        GameObjects& game_objects_;
        PlayerInfo& player_info_;
        vector<Color> colors;

        thread server_thread_;
        vector<thread> client_threads_;
        void run();
        void receiveFromPeer(string peer_id, string peer_ip);
};