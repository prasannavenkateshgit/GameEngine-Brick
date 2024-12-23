// This is the game entities for Srivathsan Govindarajan's game
#include "entity.hpp"
#include "defs.hpp"
#include "physics.hpp"
#include "timeline.hpp"
#include "game_objects.hpp"
#include "dzoneHandler.hpp"
#include "event_manager.hpp"
#include "event.hpp"
#include "input.hpp"
#include <vector>
#include <iostream>
#include <math.h>
#include <map>

extern float SCREEN_WIDTH_FACTOR, SCREEN_HEIGHT_FACTOR;
extern bool isProportional;
extern Physics *physics;
extern Timeline *global_time;
extern int64_t frameDelta;
extern std::string selfID;
extern EventManager event_manager;
extern InputEvent* input_event;
GameObjects* loadEntities(std::string clientID, Color color);
void registerEvents();

void moveEntity(GameObjects* game_objects, Uint8 *keys);

void handleDzone(GameObjects* game_objs);

void resetBricks(GameObjects* game_objects);
void update(GameObjects* game_objects);

// void patternEntity(GameObjects* game_objects);

// void handleCollision(GameObjects* game_objects);

// const extern int ROW = 5;
// const extern int COL = 7;
// const extern int BALL_SPEED = 8;
// const extern int SIZE = 16;
// const extern int SPACING = 16;
// const extern int PI = 3.14;
// const extern int SPEED = 9;

// extern bool running;
// extern int frameCount, timerFPS, lastFrame, fps;
// extern SDL_Color color;
// extern SDL_Rect paddle, ball, lives, brick;
// extern float velY, velX;
// extern int liveCount;
// extern bool bricks[ROW*COL];