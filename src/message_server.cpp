#include "message_server.hpp"
#include <chrono>
MessageServer::MessageServer(int port, GameObjects& game_objects, PlayerInfo& player_info)
    : port_(port), game_objects_(game_objects), context_(1), socket_(context_, socket_type::rep), client_count_(0), player_info_(player_info)
    {
        socket_.bind("tcp://*:"+to_string(port_));
        colors.push_back(Color{255, 0, 0});
        colors.push_back(Color{0, 255, 255});
        colors.push_back(Color{255, 0, 255});
        server_thread_ = thread(&MessageServer::run, this);
    }

MessageServer::~MessageServer()
{
    if (server_thread_.joinable()) 
    {
        server_thread_.join();
    }
    for (auto& client_thread : client_threads_) 
    {
        if (client_thread.joinable()) 
        {
            client_thread.join();
        }
    }
}


void MessageServer::receiveFromPeer(string  peer_id, string peer_ip) 
{
    context_t context(1);
    socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://"+peer_ip);
    subscriber.set(sockopt::subscribe, "");
    cout << "Connected to peer: " << peer_id << " at " << peer_ip << endl;
    while (true) {
        message_t topic_msg1, message1;
        subscriber.recv(&topic_msg1);
        string recieved_topic(static_cast<char*>(topic_msg1.data()), topic_msg1.size());
        subscriber.recv(&message1);
        string msg_str1(static_cast<char*>(message1.data()), message1.size());
        if (recieved_topic == "DISCONNECT")
        {
            // game_objects_.removeEntity(msg_str1); // topic would be "DISCONNECT" and message would be the clientID
            // cout << "DISCONNECTED: " << msg_str1 << endl;
            DisconnectionEvent* disconnection_event_peer = new DisconnectionEvent(&game_objects_, msg_str1, false, global_time->getTime(), EventPriority::HIGH);
            disconnection_event_peer->setType("DISCONNECT_PEER");
            event_manager.registerEvent(disconnection_event_peer);
            event_manager.raiseEvent(disconnection_event_peer);
        }
        else if (recieved_topic != "PLAYER_INFO" && recieved_topic.find("pattern") && recieved_topic.find("platform") != 0)
        {
            Entity* entity_ptr = game_objects_.getEntity(recieved_topic);
            Color color = entity_ptr? entity_ptr->getColor() : colors[stoi(recieved_topic) % colors.size()];
            game_objects_.updateEnitityFromString(recieved_topic, msg_str1);
            game_objects_.setEntityColor(recieved_topic, color);
        }
    }
}

void MessageServer::run()
{
    while (true) 
    {
        message_t request;
        auto result = socket_.recv(request, recv_flags::none);

        if(!result) 
        {
            std::cerr << "Error recieving message" << std::endl;
            continue;
        }
        string req_str(static_cast<char*>(request.data()), request.size());
        string reply_str;

        if (!req_str.empty() && req_str.rfind("CREATE", 0) == 0)
        {
            string clientID = to_string(client_count_);
            string client_ip = req_str.substr(req_str.find(':') + 1, req_str.size());
            player_info_.addPlayerIP(clientID, client_ip);
            thread t(&MessageServer::receiveFromPeer, this, clientID, client_ip);
            client_threads_.push_back(std::move(t));
            string timeline_str = to_string(global_time->getTime()) + "," + to_string(global_time->getTic());
            reply_str = clientID + ":" + timeline_str + ":" + to_string(colors[client_count_ % colors.size()].r) + "," + to_string(colors[client_count_ % colors.size()].g) + "," + to_string(colors[client_count_ % colors.size()].b);
            client_count_++;
            cout<<"Reply : " << reply_str <<endl;
            message_t reply(reply_str.size());
            memcpy(reply.data(), reply_str.c_str(), reply_str.size());
            socket_.send(reply, send_flags::none);
        }       

    }
}