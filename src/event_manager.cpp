#include "event_manager.hpp"
std::ofstream logFile;

EventManager::EventManager()
{
    file_pos = 0;
}

void EventManager::registerEvent(BaseEvent* event)
{
    std::string type = event->getType();
    if(event_map.find(type) == event_map.end())
    {
        event_map[type] = std::vector<BaseEvent*>();
    }
    event_map[type].push_back(event);
}

void EventManager::deregisterEvent(BaseEvent* event)
{
    std::string type = event->getType();
    if(event_map.find(type) != event_map.end())
    {
        for(int i = 0; i < event_map[type].size(); i++)
        {
            if(event_map[type][i] == event)
            {
                event_map[type].erase(event_map[type].begin() + i);
                break;
            }
        }
    }
}

void EventManager::clearEvents()
{
    event_map.clear();
}

void EventManager::raiseEvent(BaseEvent* event)
{
    std::lock_guard<std::mutex> lock(mtx);
    raised_events.push(event);
    
}

void EventManager::handleEvents()
{
    std::vector<BaseEvent*> delayed_events;
    while(!raised_events.empty())
    {
        BaseEvent* event;
        {
            std::lock_guard<std::mutex> lock(mtx);
            event = raised_events.top();
            raised_events.pop();
        }
        if (event->getDelayTimer() > 0)
        {
            event->setDelayTimer(event->getDelayTimer() - 1);
            delayed_events.push_back(event);
            continue;
        }
        std::string type = event->getType();
        if(event_map.find(type) != event_map.end())
        {
            for(int i = 0; i < event_map[type].size(); i++)
            {
                if(event_map[type][i] == event)
                {
                    event_map[type][i]->onEvent();
                    break;
                }
            }
        }
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        for(int i = 0; i < delayed_events.size(); i++)
        {
            raised_events.push(delayed_events[i]);
        }
    }
}

void EventManager::recordEvents(std::vector<RenderEvent*> events)
{
    std::ofstream logFile("render_event_log.txt", std::ios_base::app);
    if (logFile.is_open()) {
        logFile << "Time: " << global_time->getTime() << std::endl;
        for (auto event : events) {
            if (event->entity->getMovable())
            {
                logFile << selfID << ": " << event->entity->toString() << std::endl;
            }
            else
            {
                logFile << event->entity->toString() << std::endl;
            }
        }
        logFile.close();
    } else {
        std::cerr << "Unable to open log file" << std::endl;
    }
}

std::map<std::string, std::vector<BaseEvent*>> temp_event_map;
std::priority_queue<BaseEvent*> temp_raised_events;

void EventManager::playbackEvents(Camera& camera)
{
    std::ifstream logFile("render_event_log.txt");
    if (logFile.is_open()) {
        if(file_pos == 0)
        {
            std::cout << "Playback started" << std::endl;
            temp_event_map = event_map;
            temp_raised_events = raised_events;
            {    
                std::lock_guard<std::mutex> lock(mtx);
                raised_events = std::priority_queue<BaseEvent*>();
                clearEvents();
            }
        }
        logFile.seekg(file_pos);
        // std::cout << "File pos: " << file_pos << std::endl;
        std::string line;
        while (std::getline(logFile, line)) {
            if (line.find("Time: ") != std::string::npos) {
                std::string time = line.substr(6);
                file_pos = logFile.tellg();
                break;
            }
            else if(line.find(selfID + ": ") != std::string::npos)
            {
                Entity* entity = new Rect(0, 0, 30, 30, true, Color{0, 0, 0});
                entity->fromString(line.substr(selfID.size() + 2));
                SDL_Rect track;
                track.x = entity->getX();
                track.y = entity->getY();
                track.w = entity->getW();
                track.h = entity->getH();
                camera.centerOnObject(track);
                RenderEvent* render_event = new RenderEvent(entity, global_time->getTime(), EventPriority::HIGH);
                registerEvent(render_event);
                raiseEvent(render_event);
            }
            else{
                Entity* entity = new Rect(0, 0, 30, 30, false, Color{0, 0, 0});
                entity->fromString(line);
                RenderEvent* render_event = new RenderEvent(entity, global_time->getTime(), EventPriority::HIGH);
                registerEvent(render_event);
                raiseEvent(render_event);
            }
        }
    } else {
        std::cerr << "Unable to open log file" << std::endl;
    }
    if(logFile.eof())
    {
        
        std::cout << "Playback ended" << std::endl;
        // PlaybackEvent* playback_event = new PlaybackEvent("playback", global_time->getTime(), EventPriority::HIGH);
        // registerEvent(playback_event);
        // raiseEvent(playback_event);
        playback = !playback;
        // global_time->unpause();
        file_pos = 0;
        std::lock_guard<std::mutex> lock(mtx);
        event_map = temp_event_map;
        raised_events = temp_raised_events;
    }
    logFile.close();
}

EventManager event_manager;