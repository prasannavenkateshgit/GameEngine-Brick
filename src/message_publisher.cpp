#include "message_publisher.hpp"

MessagePublisher::MessagePublisher(int port, GameObjects& game_objects, PlayerInfo& player_info)
    : port_(port), game_objects_(game_objects), context_(1), publisher_(context_, ZMQ_PUB), player_info_(player_info)
{
    publisher_.bind("tcp://*:"+to_string(port));
    publisher_thread_ = std::thread(&MessagePublisher::run, this);
}

MessagePublisher::~MessagePublisher() 
{
    if (publisher_thread_.joinable()) 
    {
        publisher_thread_.join();
    }
}

void MessagePublisher::run() 
{
    while (true) 
    {
        if (isDisconnected)
        {
            const string topic_str = "DISCONNECT";
            const string msg_str = "";
            message_t topic(topic_str.data(), topic_str.size());
            // const string msg_str = game_objects->getEntityString(selfID);
            message_t message(msg_str.data(),msg_str.size());
            if (!publisher_.send(topic, zmq::send_flags::sndmore) || 
                !publisher_.send(message, zmq::send_flags::none)) {
                std::cerr << "Failed to send message!" << std::endl;
            }
            exit(0);
            break;
        }
        auto entities = game_objects_.getEntities();
        
        for(const auto& entityPair : entities)
        {
            const string topic_str = entityPair.first;
            message_t topic(topic_str.data(), topic_str.size());

            const string msg_str = entityPair.second->toString();
            message_t message(msg_str.data(),msg_str.size());
            if (!publisher_.send(topic, zmq::send_flags::sndmore) || 
                !publisher_.send(message, zmq::send_flags::none)) {
                std::cerr << "Failed to send message!" << std::endl;
            }
        }

        const string topic_str = "PLAYER_INFO";
        message_t topic(topic_str.data(), topic_str.size());

        const string msg_str = player_info_.getAllPlayerIPs();
        message_t message(msg_str.data(),msg_str.size());

        if (!publisher_.send(topic, zmq::send_flags::sndmore) || 
            !publisher_.send(message, zmq::send_flags::none)) {
            std::cerr << "Failed to send message!" << std::endl;
        }
        
    }
}