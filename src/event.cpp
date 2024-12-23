#include <event.hpp>
static std::map<std::string, int> collisions;

BaseEvent::BaseEvent(EventPriority priority, int64_t timestamp)
{
    this->priority = priority;
    this->timestamp = timestamp;
    this->delay_timer = 0;
    type = typeid(*this).name();
    subclass_events.push_back(typeid(*this).name());
}

int64_t BaseEvent::getTimestamp()
{
    return this->timestamp;
}

std::string BaseEvent::getType()
{
    return this->type;
}

EventPriority BaseEvent::getPriority()
{
    return this->priority;
}

void BaseEvent::setPriority(EventPriority priority)
{
    this->priority = priority;
}

void BaseEvent::setType(std::string type)
{
    this->type = type;
}

bool BaseEvent::operator<(const BaseEvent &other) const
{
    return this->priority < other.priority;
}

void BaseEvent::setDelayTimer(Uint64 delay_timer)
{
    this->delay_timer = delay_timer;
}

Uint64 BaseEvent::getDelayTimer()
{
    return this->delay_timer;
}

DisconnectionEvent::DisconnectionEvent(GameObjects* game_objects, std::string clientID, bool self, int64_t timestamp, EventPriority priority) : BaseEvent(priority, timestamp)
{
    this->game_objects = game_objects;
    this->clientID = clientID;
    this->self = self;
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = handle_keyboard_interrupt;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
}

void DisconnectionEvent::handle_keyboard_interrupt(int s)
{
    isDisconnected = true;
}

void DisconnectionEvent::onEvent()
{
    if (this->game_objects->getEntity(this->clientID) != nullptr && !self)
    {
        this->game_objects->removeEntity(this->clientID);
        std::cout << "DISCONNECTED: " << this->clientID << std::endl;
    }
    else if(self  || clientID == "server")
    {
        isDisconnected = true;
    }
}

InputEvent::InputEvent(std::function<void(GameObjects*, Uint8*)> callback, GameObjects* game_objects, int64_t timestamp, EventPriority priority) : BaseEvent(priority, timestamp)
{
    this->callback = callback;
    this->game_objects = game_objects;
}

void InputEvent::onEvent()
{
    Uint8 const *keys;
    Uint8 valid_keys[SDL_NUM_SCANCODES] = {0};
    keys = SDL_GetKeyboardState(nullptr);
    float current_time = global_time->getTime();
    if(global_time->isPaused())
    {
        current_time += global_time->getPausedTime();
    }
    for (size_t i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        if(keys[i] == 1)
        {
            if(current_time - previous_key_presses[i] >= KEY_REPEAT_DELAY * frameDelta)
            {
                valid_keys[i] = 1;
                previous_key_presses[i] = current_time;
            }
        }   
    }
    this->callback(game_objects, valid_keys);
}


DeathZoneEvent::DeathZoneEvent(Coord spawn, Rect* player, int64_t timestamp, EventPriority priority)
    : BaseEvent(priority, timestamp), player(player), spawn(spawn) {}

void DeathZoneEvent::onEvent()
{
    player->setX(spawn.x);
    player->setY(spawn.y);
}


CollisionEvent::CollisionEvent(std::function<void(int)> callback, GameObjects* game_objects, std::string entity1, std::string entity2, int64_t timestamp, EventPriority priority) : BaseEvent(priority, timestamp)
{
    this->callback = callback;
    this->game_objects = game_objects;
    this->entity1 = entity1;
    this->entity2 = entity2;
}

void CollisionEvent::onEvent()
{
    std::cout << "Collision between " << this->entity1 << " and " << this->entity2 << std::endl;
    if (collisions.find(entity2) == collisions.end()) {
        collisions[entity2] = 1;
    }
    else {
        collisions[entity2] += 1;
        std::cout << "Collision count for " << entity2 << " is " << collisions[entity2] << std::endl;
    }
    this->callback(collisions[entity2]);

}

RenderEvent::RenderEvent(Entity *entity, int64_t timestamp, EventPriority priority): BaseEvent(priority, timestamp)
{
    this->entity = entity;
}

void RenderEvent::onEvent()
{
    if (entity->getX() + entity->getW() > camera.viewRect.x &&
        entity->getX() < camera.viewRect.x + camera.viewRect.w &&
        entity->getY() + entity->getH() > camera.viewRect.y &&
        entity->getY() < camera.viewRect.y + camera.viewRect.h)
    {
        //entity->drawEntity(app->renderer);
        entity->drawEntityCamera(app->renderer, camera.viewRect.x, camera.viewRect.y);
    }
}

bool recording = false;

ReplayEvent::ReplayEvent(std::string replay, int64_t timestamp, EventPriority priority): BaseEvent(priority, timestamp)
{
    this->replay_file_name = replay;
}

void ReplayEvent::onEvent()
{
    recording = !recording;
}

bool playback = false;

PlaybackEvent::PlaybackEvent(std::string replay, int64_t timestamp, EventPriority priority): BaseEvent(priority, timestamp)
{
    this->replay_file_name = replay;
}

void PlaybackEvent::onEvent()
{
    playback = !playback;
    // if (playback) {
    //     global_time->pause();
    // }
    // else {
    //     global_time->unpause();
    // }
}
