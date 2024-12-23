#pragma once 
#include "entity.hpp"
#include "defs.hpp"
#include "timeline.hpp"
#include "game_objects.hpp"
#include "event_manager.hpp"
#include "event.hpp"
#include "networking.hpp"
#include <future>
#include <iostream>
#include <map>
#include <functional>
#include <signal.h>
#include <SDL2/SDL.h>

extern float SCREEN_WIDTH_FACTOR, SCREEN_HEIGHT_FACTOR;
extern bool isProportional;
extern Timeline *global_time;
extern int64_t frameDelta;
extern bool isDisconnected;
extern DisconnectionEvent* disconnection_event;
extern InputEvent* input_event;
extern EventManager event_manager;


void handle_keyboard_interrupt(int s);

void doInput(GameObjects* game_objects);