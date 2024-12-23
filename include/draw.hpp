#pragma once
#include "structs.hpp"
#include "defs.hpp"
#include "entity.hpp"
#include "physics.hpp"
#include "game_objects.hpp"
#include "camera.hpp"
#include "event_manager.hpp"
#include "event.hpp"
#include <iostream>
#include <map>
#include <SDL2/SDL.h>

// SDL render and window context
extern App* app;
extern float SCREEN_WIDTH_FACTOR, SCREEN_HEIGHT_FACTOR;
extern bool isProportional;
extern Physics* physics;
extern EventManager event_manager;
extern Timeline *global_time;
extern bool recording;
extern bool playback;

// extern struct Camera camera;

// Prepare scene to be rendered to window
void prepareScene(void);
// Render scene to window
// extern struct Camera camera;

void presentScene(GameObjects* game_objects, std::string selfID);