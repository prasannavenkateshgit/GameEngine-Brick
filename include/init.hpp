#pragma once
#include "structs.hpp"
#include "defs.hpp"
#include <SDL2/SDL.h>
#include <iostream>

// SDL render and window context
extern App* app;

extern float SCREEN_WIDTH_FACTOR, SCREEN_HEIGHT_FACTOR;

// Initialize SDL rendering window
void initSDL(void);