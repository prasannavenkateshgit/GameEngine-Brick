#pragma once
#include "defs.hpp"
#include <threads.h>
#include <numeric>
#include <vector>
#include <mutex>
#include <chrono>
#include <cmath>
#include <iostream>

class Timeline
{
    private:
        std::mutex m;
        int64_t start_time; //the time of the *anchor when created
        int64_t elapsed_paused_time;
        int64_t last_paused_time;
        float tic; //units of anchor timeline per step
        bool paused;
        Timeline *anchor; //for most general game time, system library pointer
    public:
        Timeline(Timeline *anchor, float tic);
        Timeline(float anchor_start_time, float anchor_tic, float tic); 
        Timeline(); //optional, may not be included
        int64_t getTime(); //this can be game or system time implementation
        void pause();
        void unpause();
        float getTic();
        void changeTic(float tic); //optional
        bool isPaused(); //optional
        int64_t getPausedTime();
};

void updateMovingAverageFrameDelta(int64_t frameDelta);