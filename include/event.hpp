#pragma once
#include "structs.hpp"
#include "defs.hpp"
#include "entity.hpp"
#include "physics.hpp"
#include "game_objects.hpp"
#include "camera.hpp"
#include "structs.hpp"
#include <iostream>
#include <map>
#include <functional>
#include <memory>
#include <csignal>
#include <SDL2/SDL.h>
//#include "game.hpp"

enum EventPriority
{
    LOW = 0,
    MEDIUM = 1,
    HIGH = 2
};

class BaseEvent
{
    private:
        int64_t timestamp;
        EventPriority priority;
        std::string type;
        Uint64 delay_timer;
    public:
        std::vector<std::string> subclass_events;
        BaseEvent(EventPriority priority, int64_t timestamp);
        virtual void onEvent() = 0;   
        int64_t getTimestamp();
        std::string getType();
        EventPriority getPriority();
        void setPriority(EventPriority priority);
        void setType(std::string type);
        bool operator<(const BaseEvent& other) const;
        void setDelayTimer(Uint64 delay_timer);
        Uint64 getDelayTimer();
};

class CollisionEvent : public BaseEvent
{
    private:
        std::function<void(int)> callback;
        GameObjects* game_objects;
        std::string entity1;
        std::string entity2;
    public:
        //CollisionEvent(std::function<void(GameObjects*, std::string, std::string)> callback, int64_t timestamp, EventPriority priority);
        CollisionEvent(std::function<void(int)> callback, GameObjects* game_objects, std::string entity1, std::string entity2, int64_t timestamp, EventPriority priority);
        void onEvent();
};


class DeathZoneEvent : public BaseEvent
{
    private:
        Rect *player;
        Coord spawn;
    public:
        DeathZoneEvent(Coord spawn, Rect *player, int64_t timestamp, EventPriority priority);
        void onEvent();
};

class DisconnectionEvent : public BaseEvent
{
    private:
        GameObjects* game_objects;
        std::string clientID;
        bool self;
        static void handle_keyboard_interrupt(int s);
    public:
        DisconnectionEvent(GameObjects* game_objects, std::string clientID, bool self, int64_t timestamp, EventPriority priority);
        void onEvent();
};

class InputEvent : public BaseEvent
{
    private:
        std::function<void(GameObjects*, Uint8* )> callback;
        GameObjects* game_objects;
        int previous_key_presses[SDL_NUM_SCANCODES] = {0};
        const float KEY_REPEAT_DELAY = 5.0;
    public:
        InputEvent(std::function<void(GameObjects*, Uint8*)> callback, GameObjects* game_objects, int64_t timestamp, EventPriority priority);
        void onEvent();
};

class RenderEvent : public BaseEvent
{
    public:
        Entity* entity;
        RenderEvent(Entity* entity, int64_t timestamp, EventPriority priority);
        void onEvent();
}; 

class ReplayEvent : public BaseEvent
{
    private:
        std::string replay_file_name;
    public:
        ReplayEvent(std::string replay, int64_t timestamp, EventPriority priority);
        void onEvent();
};

class PlaybackEvent: public BaseEvent
{
    private:
        std::string replay_file_name;
    public:
        PlaybackEvent(std::string replay, int64_t timestamp, EventPriority priority);
        void onEvent();
};

extern bool isDisconnected;
extern int64_t frameDelta;
extern Timeline *global_time;
extern App* app;
extern bool recording;
extern bool playback;