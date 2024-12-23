#pragma once
#include "timeline.hpp"
#include "structs.hpp"
#include "entity.hpp"
#include<cmath>
#include <SDL2/SDL.h>
#include "game_objects.hpp"

extern Timeline *global_time;
extern int64_t frameDelta;

class Physics {
    private:
        float gravity;

    public:
        Physics(float initialGravity);

        void applyGravity(Entity* entity, std::string EntityName, GameObjects* game_objs);

        float getGravity();
        void setGravity(float g);
        std::map<std::string, Entity*> getElementsStartingWith(const std::map<std::string, Entity*>& game_objs, const std::string& prefix);
};

bool detectCollision(Entity* ent1, Entity* ent2);