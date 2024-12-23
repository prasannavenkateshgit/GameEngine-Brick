#include "game_objects.hpp"

GameObjects::GameObjects()
{
    this->timeline = global_time;
}

std::mutex& GameObjects::GetMutexForEntity(const std::string& name)
{
   if (entity_mutex_map.find(name) == entity_mutex_map.end()) {
        entity_mutex_map[name] = std::make_unique<std::mutex>();
    }
    return *entity_mutex_map[name];
}

void GameObjects::addEntity(std::string name, Entity* entity)
{
    // if(this->entities.find(name) == this->entities.end())
    // {
        
    //     // entity_mutex_map.emplace(std::piecewise_construct, std::make_tuple(name), std::make_tuple());
    // }
    std::lock_guard<std::mutex> lock(game_mutex);
    this->entities[name] = entity;

}

void GameObjects::removeEntity(std::string name)
{
    std::lock_guard<std::mutex> lock(game_mutex);
    this->entities.erase(name);
    entity_mutex_map.erase(name);
}

Entity* GameObjects::getEntity(std::string name)
{
    if(this->entities.find(name) != this->entities.end())
    {
        return this->entities[name];
    }
    return nullptr;
}

void GameObjects::moveEntity(std::string name, bool absolute)
{
    std::lock_guard<std::mutex> lock(GetMutexForEntity(name));
    this->entities[name]->move(this->timeline->getTic(), absolute);
}

void GameObjects::setEntityVelocityX(std::string name, float vel_x)
{
    std::lock_guard<std::mutex> lock(GetMutexForEntity(name));
    this->entities[name]->setVelocityX(vel_x);
}

void GameObjects::setEntityVelocityY(std::string name, float vel_y)
{
    std::lock_guard<std::mutex> lock(GetMutexForEntity(name));
    this->entities[name]->setVelocityY(vel_y);
}

void GameObjects::setEntityColor(std::string name, Color color)
{
    std::lock_guard<std::mutex> lock(GetMutexForEntity(name));
    this->entities[name]->setColor(color);
}

void GameObjects::scaleEntity(std::string name)
{
    std::lock_guard<std::mutex> lock(GetMutexForEntity(name));
    this->entities[name]->scale();
}

void GameObjects::scaleAllEntities()
{
    // std::lock_guard<std::mutex> lock(game_mutex);
    for(auto ent: this->entities)
    {
        this->scaleEntity(ent.first);
    }
}

void GameObjects::linkTimeline(Timeline* timeline)
{
    this->timeline = timeline;
}

std::string GameObjects::getEntityString(std::string name)
{
    return this->entities[name]->toString();
}

void GameObjects::updateEnitityFromString(std::string name, std::string str)
{
    if(this->entities.find(name) == this->entities.end())
    {
        this->addEntity(name, new Rect(0, 0, 30, 30, false, Color{0, 0, 0}));
    }
    this->entities[name]->fromString(str);
}

GameObjects::~GameObjects()
{
    for(auto ent: this->entities)
    {
        delete ent.second;
    }
}

std::map<std::string, Entity *> GameObjects::getEntities()
{
    return this->entities;
}
