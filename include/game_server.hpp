#include "game_objects.hpp"
#include "defs.hpp"
#include "physics.hpp"
#include "event.hpp"
#include "event_manager.hpp"
#include <vector>
#include <string>

extern Timeline *global_time;
extern CollisionEvent* collision_event;
extern EventManager event_manager;
extern int modifier;

GameObjects* loadServerEntities(void);
void patternEntity(GameObjects& game_objects);
void handleCollision(GameObjects& game_objects, std::string cobj);
std::map<std::string, Entity*> getElementsStartingWith(const std::map<std::string, Entity*>& game_objs, const std::string& prefix);
int scoreModifier(int score);
int getModifier();