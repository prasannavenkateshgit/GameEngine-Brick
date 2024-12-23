#include "physics.hpp"
#include <iostream>
using namespace std;

Physics::Physics(float initialGravity=9.8f) {
    gravity = initialGravity;
}

/**
 * Applies gravity to the given Entity, by updating its vertical velocity and position.
 * If the given Entity is colliding with the given platform, it will be moved to the
 * top of the platform and its vertical velocity will be set to 0.
 *
 * @param entity The Entity to apply gravity to.
 * @param platform The Entity to check for collision with. If NULL, no collision
 * detection is performed.
 */
void Physics::applyGravity(Entity* entity, std::string EntityName, GameObjects* game_objects) {

    if(entity->underGravity && !global_time->isPaused())
    {
        // Uint32 t = SDL_GetTicks();
        // float dt = (t - entity->getTime())/1000.0f;
        float dt =  frameDelta /(100000.0f);
        float delta_y = entity->getVelocityY() * dt + 0.5f * gravity * dt * dt;
        float v = entity->getVelocityY() +gravity*dt;
        // float v = sqrtf( (entity->getVelocityY() *  entity->getVelocityY() + 2 * gravity * delta_y));
        entity->setVelocityY(v);
        entity -> move(1, false);

        std::map<std::string, Entity*> platforms = getElementsStartingWith(game_objects->getEntities(), "platform");
        for (const auto& platform : platforms)
        {
            if(detectCollision(entity, platform.second))
            {   
                float new_delta_y = entity->getY() + entity->getH() - platform.second->getY();
                entity->setVelocityY((delta_y - new_delta_y));
                entity -> move(1, false);
                entity->setVelocityY(0.0f);
            }
        }
        
        std::map<std::string, Entity*> mov_platforms = getElementsStartingWith(game_objects->getEntities(), "mov");

        for(const auto& mov_platform : mov_platforms)
        {
            if(detectCollision(entity, mov_platform.second))
            {   
                // std::cout << "Collion between H platform and entity" << std::endl;
                entity->setX(mov_platform.second->getX());
                entity->setVelocityX(mov_platform.second->getVelocityX());
                entity->setVelocityY(mov_platform.second->getVelocityY());
                entity->setY(mov_platform.second->getY()-entity->getH());                
            }
        }

    }
}

std::map<std::string, Entity*> Physics::getElementsStartingWith(const std::map<std::string, Entity*>& game_objs, const std::string& prefix) {
    std::map<std::string, Entity*> platforms;
    for (const auto& pair : game_objs) {
        if (pair.first.find(prefix) == 0) {
            platforms.insert(pair);
        }
    }
    return platforms;
}

float Physics::getGravity() {
    return gravity;
}

void Physics::setGravity(float g) {
    gravity = g;
}

Physics *physics;

/**
 * Checks if two entities are colliding.
 *
 * This function takes two entities, and checks if their bounding boxes
 * intersect. It returns true if the entities are colliding, and false if
 * they are not.
 *
 * @param ent1 The first entity to check for collision.
 * @param ent2 The second entity to check for collision.
 * @return true if the entities are colliding, false otherwise.
 */
bool detectCollision(Entity *ent1, Entity *ent2)
{
    SDL_Rect rect1 = dynamic_cast<Rect&>(*ent1).getRect();
    SDL_Rect rect2 = dynamic_cast<Rect&>(*ent2).getRect();
    return SDL_HasIntersection(&rect1, &rect2);
}
