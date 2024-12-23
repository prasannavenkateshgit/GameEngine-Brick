#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <sstream>
#include <iostream>

typedef struct {
	int r, g, b;
} Color;

typedef struct {
	float x, y;
} Coord;

struct GameItem
{
    Coord position;
    Color color;

    std::string toString(void) const;

    static GameItem toGameItem(const std::string& str);
};


//A struct to contain the pointers to the SDL Renderer and SDL window.
typedef struct {
	SDL_Renderer* renderer;
	SDL_Window* window;
} App;

// extern Uint32 fps;
extern Uint32 frameDuration;