#pragma once
#include "defs.hpp"
#include <SDL2/SDL.h>

struct Camera
{
    SDL_Rect viewRect;
    Camera(int x, int y, int w, int h);
    void centerOnObject(SDL_Rect& target);
};

extern Camera camera;

