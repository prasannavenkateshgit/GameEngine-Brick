#pragma once
#include "entity.hpp"
#include "defs.hpp"
#include "timeline.hpp"
#include <string>
#include <thread>
#include <mutex>
#include <map>

class GameObjects
{
    public:
        GameObjects();
        ~GameObjects();
        std::mutex& GetMutexForEntity(const std::string& name);
        std::map<std::string, Entity*> getEntities();
        void addEntity(std::string name, Entity* entity);
        void removeEntity(std::string name);
        Entity* getEntity(std::string name);
        void moveEntity(std::string name, bool absolute);
        void setEntityVelocityX(std::string name, float vel_x);
        void setEntityVelocityY(std::string name, float vel_y);
        void setEntityColor(std::string name, Color color);
        void scaleEntity(std::string name);
        void scaleAllEntities();
        void linkTimeline(Timeline* timeline);
        std::string getEntityString(std::string name);
        void updateEnitityFromString(std::string name, std::string str);
    private:
        std::map<std::string, Entity*> entities;
        std::mutex game_mutex;
        std::map<std::string, std::unique_ptr<std::mutex>> entity_mutex_map;
        Timeline* timeline;
};

extern Timeline *global_time;