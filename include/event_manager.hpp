#pragma once
#include "structs.hpp"
#include "defs.hpp"
#include "entity.hpp"
#include "physics.hpp"
#include "game_objects.hpp"
#include "event.hpp"
#include "camera.hpp"
#include <iostream>
#include <map>
#include <functional>
#include <typeinfo>
#include <queue>
#include <mutex>
#include <SDL2/SDL.h>
#include <fstream>

class EventManager
{
    public:
        EventManager();
        void registerEvent(BaseEvent* event);
        void deregisterEvent(BaseEvent* event);
        void clearEvents();
        void raiseEvent(BaseEvent* event);
        void handleEvents();
        void recordEvents(std::vector<RenderEvent*> events);
        void playbackEvents(Camera& camera);
    private:
        std::map<std::string, std::vector<BaseEvent*>> event_map;
        std::priority_queue<BaseEvent*> raised_events;
        std::mutex mtx;
        std::streampos file_pos;
        void setFilePos(std::streampos pos);
};

extern EventManager event_manager;
extern bool recording;
extern bool playback;
extern std::string selfID;