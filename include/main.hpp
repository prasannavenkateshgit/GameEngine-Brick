#pragma once
#include "structs.hpp"
#include "init.hpp"
#include "networking.hpp"
#include "dzoneHandler.hpp"

// SDL render and window context
extern App* app;
extern Physics* physics;
extern Timeline *global_time;
extern int64_t frameDelta;
extern float SCREEN_WIDTH_FACTOR, SCREEN_HEIGHT_FACTOR;
extern bool isProportional;
extern bool isDisconnected;


int main(int argc, char* argv[]);