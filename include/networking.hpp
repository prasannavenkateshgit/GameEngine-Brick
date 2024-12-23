#pragma once
#include "structs.hpp"
#include "init.hpp"
#include "draw.hpp"
#include "input.hpp"
#include "game.hpp"
#include "game_server.hpp"
#include "message_publisher.hpp"
#include "message_server.hpp"
#include "game_objects.hpp"
#include "players_info.hpp"
#include "dzoneHandler.hpp"
#include "event_manager.hpp"
#include "event.hpp"
#include <future>
#include <iostream>
#include <memory>
#include <unordered_set>
#include <cstring>

// SDL render and window context
extern App* app;
extern Physics* physics;
extern Timeline *global_time;
extern int64_t frameDelta;
extern float SCREEN_WIDTH_FACTOR, SCREEN_HEIGHT_FACTOR;
extern bool isProportional;
extern bool isDisconnected;
extern std::string self_id;
extern EventManager event_manager;
extern DisconnectionEvent* disconnection_event;

void server_loop();
void client_loop(string server_ip, string self_ip, string self_pubport);
tuple<string, Color> handshake(Timeline* local_time, string server_ip, string self_ip, string self_pubport);
void sendToServer(GameObjects* game_objects, string clientID, string server_ip);
void receiveFromServer(GameObjects* game_objects, string self_clientID, string server_ip, PlayerInfo* player_info);
void sendToPeers(GameObjects* game_objects, string selfID, string port);
void receiveFromPeer(GameObjects* game_objects, string self_clientID, string peer_id, string peer_ip, string peer_port);
void sendDisconnectMessage(string self_clientID, string port);
void checkWin(GameObjects* game_objects, string self_clientID);
