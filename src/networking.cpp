#include "networking.hpp"

std::chrono::steady_clock::time_point start_time;
DisconnectionEvent* disconnection_event;
std::string self_id;

tuple<string, Color> handshake(Timeline* local_time, string server_ip, string self_ip, string self_pubport)
{
    context_t context(1);
    socket_t socket (context, socket_type::req);
    socket.connect("tcp://"+server_ip+":5555");
    string req_str = "CREATE:"+self_ip+":"+self_pubport;
    message_t request(req_str.size()), message;
    memcpy(request.data(), req_str.c_str(), req_str.size());
    socket.send(request);
    socket.recv(&message);
    string msg_str(static_cast<char*>(message.data()), message.size());
    cout << "REPLY : " << msg_str << endl;
    cout << "YOU HAVE 2 LIVES! DESTROY ALL BRICKS!" << endl;
    start_time = std::chrono::steady_clock::now();
    size_t colonPos = msg_str.find(':'), commaPos = msg_str.find(',');
    string clientID = msg_str.substr(0, colonPos);
    float time = std::stof(msg_str.substr(colonPos + 1, commaPos - colonPos - 1));
    float tic = std::stof(msg_str.substr(commaPos + 1, msg_str.size() - commaPos - 1));
    local_time = new Timeline(time, tic, 1.0);
    // get color from msg_str
    Color color;
    size_t secondColonPos = msg_str.find(':', colonPos + 1);
    std::istringstream iss(msg_str.substr(secondColonPos + 1, msg_str.size() - secondColonPos - 1));
    std::string token;
    std::getline(iss, token,  ',');
    color.r = std::stoi(token);
    std::getline(iss, token,  ',');
    color.g = std::stoi(token);
    std::getline(iss, token);
    color.b = std::stoi(token);
    return std::make_tuple(clientID, color);
}

void sendToPeers(GameObjects* game_objects, string selfID, string port) {
    
    context_t context(1);
    socket_t publisher (context, socket_type::pub);
    // std::cout << "Publishing on port: " << port << std::endl;
    publisher.bind("tcp://*:"+port);
    // std::cout << "Publishing on port: close " << port << std::endl;
    while (true) {
        string topic_str, msg_str;
        if (isDisconnected)
        {
            topic_str = "DISCONNECT";
            msg_str = selfID;
            message_t topic(topic_str.data(), topic_str.size());
            // const string msg_str = game_objects->getEntityString(selfID);
            message_t message(msg_str.data(),msg_str.size());
            if (!publisher.send(topic, zmq::send_flags::sndmore) || 
                !publisher.send(message, zmq::send_flags::none)) {
                std::cerr << "Failed to send message!" << std::endl;
            }
            exit(0);
            break;
        }
        else
        {
            topic_str = selfID;
            msg_str = game_objects->getEntityString(selfID);
            // const string topic_str = selfID;
            message_t topic(topic_str.data(), topic_str.size());
            // const string msg_str = game_objects->getEntityString(selfID);
            message_t message(msg_str.data(),msg_str.size());
            if (!publisher.send(topic, zmq::send_flags::sndmore) || 
                !publisher.send(message, zmq::send_flags::none)) {
                std::cerr << "Failed to send message!" << std::endl;
            }
        }
    }
}

void sendToServer(GameObjects* game_objects, string clientID, string server_ip) {
    
    context_t context(1);
    socket_t socket (context, socket_type::req);
    socket.connect("tcp://"+server_ip+":5555");
    while (true) {
        string req_str = "";
        req_str = clientID+ ":" + game_objects->getEntityString(clientID);
        message_t request(req_str.size()), message;
        memcpy(request.data(), req_str.c_str(), req_str.size());
        socket.send(request);
        socket.recv(&message);
        string msg_str(static_cast<char*>(message.data()), message.size());
    }
}

void receiveFromPeer(GameObjects* game_objects, string self_clientID, string peer_id, string peer_ip, string peer_port) 
{
    context_t context(1);
    socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://"+peer_ip+":"+peer_port);
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
            // game_objects->removeEntity(msg_str1); // topic would be "DISCONNECT" and message would be the clientID
            // cout << "DISCONNECTED: " << msg_str1 << endl;
            DisconnectionEvent* disconnection_event_peer = new DisconnectionEvent(game_objects, msg_str1, false, global_time->getTime(), EventPriority::HIGH);
            disconnection_event_peer->setType("DISCONNECT_PEER");
            event_manager.registerEvent(disconnection_event_peer);
            event_manager.raiseEvent(disconnection_event_peer);
        }
        else if (self_clientID != recieved_topic && recieved_topic.find("platform") !=0 )
        {
            game_objects->updateEnitityFromString(recieved_topic, msg_str1); // topic would be the clientID and message would be the entity string
        }
    }
}

void receiveFromServer(GameObjects* game_objects, string self_clientID, string server_ip, PlayerInfo* player_info) 
{
    
    context_t context(1);
    socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://"+server_ip+":5556");
    subscriber.set(sockopt::subscribe, "");
    std::unordered_set<std::string> clients_connected_list = {};
    clients_connected_list.insert(self_clientID);
    std::vector<std::thread> client_threads;
    while (true) {
        message_t topic_msg1, message1;

        subscriber.recv(&topic_msg1);
        string recieved_clientID(static_cast<char*>(topic_msg1.data()), topic_msg1.size());
        subscriber.recv(&message1);
        string msg_str1(static_cast<char*>(message1.data()), message1.size());

        if(recieved_clientID == "DISCONNECT")
        {
            cout << "Disconnecting from game due to server exit" << endl;
            DisconnectionEvent* disconnection_event_server = new DisconnectionEvent(game_objects, std::string("server"), true, global_time->getTime(), EventPriority::HIGH);
            disconnection_event_server->setType("DISCONNECT_SERVER");
            event_manager.registerEvent(disconnection_event_server);
            event_manager.raiseEvent(disconnection_event_server);
            break;
        }
        else if(recieved_clientID == "PLAYER_INFO")
        {
            player_info->updatePlayerIPs(msg_str1);
            auto clientMap = player_info->getPlayerIPMap();
            for(auto& client : clientMap)
            {
                if(clients_connected_list.find(client.first) == clients_connected_list.end())
                {
                    clients_connected_list.insert(client.first);
                    std::string peer_ip = client.second.substr(0, client.second.find(":"));
                    std::string peer_port = client.second.substr(client.second.find(":")+1, client.second.size());

                    // std::cout << "Connecting to peer: " << client.first << " at " << peer_ip << ":" << peer_port << std::endl;
                    // std::thread t();
                    std::thread t(receiveFromPeer,game_objects, self_clientID, client.first, peer_ip, peer_port);
                    client_threads.push_back(std::move(t));
                }
            }

        }
        else
        {
            // if (recieved_clientID.find("ball") ==0)
            // {
            //     game_objects->updateEnitityFromString(recieved_clientID, msg_str1);
            // }
            if (recieved_clientID == self_clientID)
            {
                // update only the color if its the same client
                Color color;
                std::istringstream iss(msg_str1);
                std::string token;
                std::getline(iss, token,  ','); // x
                std::getline(iss, token,  ','); // y
                std::getline(iss, token,  ','); // w
                std::getline(iss, token,  ','); // h
                std::getline(iss, token,  ',');
                color.r = std::stoi(token);
                std::getline(iss, token,  ',');
                color.g = std::stoi(token);
                std::getline(iss, token);
                color.b = std::stoi(token);
                //game_objects->setEntityColor(self_clientID, color);
            }
        }
    }
    for(auto& t: client_threads)
    {
        if(t.joinable())
        {
            t.join();
        }
    }
}

void handle_keyboard_interrupt(int s)
{
    isDisconnected = true;
}

void register_signal_handler()
{
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = handle_keyboard_interrupt;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
}

void server_loop()
{
    auto game_objects_ptr = loadServerEntities();
    PlayerInfo* player_info =  new PlayerInfo();
    GameObjects& game_objects = *game_objects_ptr;
    global_time = new Timeline();
    MessageServer server(5555, game_objects, *player_info);
    MessagePublisher publisher(5556, game_objects, *player_info);
    this_thread::sleep_for(chrono::milliseconds(1000)); // sync time
    register_signal_handler();
    disconnection_event = new DisconnectionEvent(game_objects_ptr, std::string("server"), false, global_time->getTime(), EventPriority::HIGH);
    disconnection_event->setType("DISCONNECT_SERVER");
    event_manager.registerEvent(disconnection_event);
    while (true) {
        patternEntity(game_objects);
        event_manager.handleEvents();
        SDL_Delay(MIN_FRAME_DURATION/global_time->getTic());
    }
}

void checkWin(GameObjects* game_objects, string self_clientID)
{
    SDL_Rect final = dynamic_cast<Rect*>(game_objects->getEntity("pattern4"))->getRect();
    SDL_Rect client = dynamic_cast<Rect*>(game_objects->getEntity(self_clientID))->getRect();
    if (SDL_HasIntersection(&final, &client)){
        std::cout << "YOU WIN" << std::endl;
        std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_time = end_time - start_time;
        std::cout << "Game time: " << elapsed_time.count() << " seconds" << std::endl;
        int modifier = getModifier();
        double score = 1000.0 / (elapsed_time.count() * (1.0 + modifier / 100.0));
        std::cout << "Score: " << score << std::endl;
        exit(0);
    }
}

void client_loop(string server_ip, string self_ip, string self_pubport)
{
    // std::cout << " Client loop entered !!" << std::endl;
    Timeline* local_time;
    PlayerInfo* player_info =  new PlayerInfo();
    auto [selfID, color] = handshake(local_time, server_ip, self_ip, self_pubport);
    global_time = local_time;
    self_id = selfID;
    initSDL();
    auto game_objects = loadEntities(selfID, color);
    std::cout<<"Loaded entities"<<endl;
    int size = game_objects->getEntities().size();
    cout<<game_objects->getEntity(selfID)<<endl;
    cout<<"Number of entities: "<<size<<endl;
    // thread t1(sendToServer, game_objects, selfID, server_ip);
    //thread receive_from_server_thread([&]() { receiveFromServer(game_objects, selfID, server_ip, player_info); });
    //thread send_to_peers_thread(sendToPeers, game_objects, selfID, self_pubport);
    // thread t4([&]() { receiveFromPeer(game_objects, player_info, selfID); });
    disconnection_event = new DisconnectionEvent(game_objects, selfID, true, global_time->getTime(), EventPriority::HIGH);
    disconnection_event->setType("DISCONNECT_CLIENT");
    event_manager.registerEvent(disconnection_event);
    this_thread::sleep_for(chrono::milliseconds(1000)); // sync time
    int64_t lastTime = global_time->getTime();
    register_signal_handler();
    //handleDzone(game_objects);
    resetBricks(game_objects);
    
    while (1)
    {
        if(isDisconnected)
        {
            break;
        }
        //Prep the scene
        prepareScene();

        //Process input
        doInput(game_objects);
        update(game_objects);

        //Present the resulting scene
        presentScene(game_objects, selfID);
        //event_manager.handleEvents();
        //SDL_RenderPresent(app->renderer);

        //Inserting 16ms delay at the end for a budget frame-limiter.
        SDL_Delay(float(MIN_FRAME_DURATION) / global_time->getTic()); 
        int64_t currentTime = global_time->getTime();
        updateMovingAverageFrameDelta(currentTime - lastTime);
        lastTime = currentTime;
        //checkWin(game_objects, selfID);
    }
    event_manager.clearEvents();
    DzoneHandler::getInstance().stopCollisionChecking();
    //receive_from_server_thread.join();
    //send_to_peers_thread.join();
}
