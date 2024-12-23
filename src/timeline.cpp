#include "timeline.hpp"

// Helper function to get the current system time in microseconds
static int64_t getSystemTime() {
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
    return duration.count();
}

Timeline::Timeline(Timeline *anchor, float tic) : anchor(anchor), tic(tic), paused(false), elapsed_paused_time(0) {
    if(anchor)
    {
        start_time = anchor->getTime() * anchor->getTic();
    }
    else
    {
        start_time = getSystemTime();
    }
}

Timeline::Timeline(float anchor_start_time, float anchor_tic, float tic): anchor(nullptr), tic(tic), paused(false), elapsed_paused_time(0)
{
    start_time = anchor_start_time * anchor_tic;
}

Timeline::Timeline() : Timeline(nullptr, 1) {}

int64_t Timeline::getTime() {
    std::scoped_lock<std::mutex> lock(m);
    int64_t elapsed_time;

    if (paused) {
        elapsed_time =  last_paused_time - start_time - elapsed_paused_time;
    }
    else
    {
        int64_t current_time = getSystemTime();
        elapsed_time = current_time - start_time - elapsed_paused_time;
    }

    return elapsed_time / static_cast<int>(round(tic));
}

void Timeline::pause() {
    std::scoped_lock<std::mutex> lock(m);
    if (!paused) {
        last_paused_time = getSystemTime();
        paused = true;
    }
}

void Timeline::unpause() {
    std::scoped_lock<std::mutex> lock(m);
    if (paused) {
        elapsed_paused_time += getSystemTime() - last_paused_time;
        paused = false;
    }
}

float Timeline::getTic()
{
    std::scoped_lock<std::mutex> lock(m);
    if(paused)
    {
        return 0;
    }
    return tic;
}

void Timeline::changeTic(float newTic) {
    std::scoped_lock<std::mutex> lock(m);
    if (tic != newTic) {
        int64_t current_time = getSystemTime() - start_time - elapsed_paused_time;
        tic = newTic < 0.5? 0.5 : newTic > 2.0 ? 2.0 : newTic;
        start_time = getSystemTime() - current_time;
    }
}

bool Timeline::isPaused() {
    std::scoped_lock<std::mutex> lock(m);
    return paused;
}

int64_t Timeline::getPausedTime()
{
    std::scoped_lock<std::mutex> lock(m);
    if(paused)
    {
        return getSystemTime() - last_paused_time;
    }
    else{
        return elapsed_paused_time;
    }
}

Timeline *global_time;
int64_t frameDelta;
std::vector<int64_t> frameDeltas;
void updateMovingAverageFrameDelta(int64_t newframeDelta)
{
    if(frameDeltas.size() < MOVING_AVG_LEN)
    {
        frameDeltas.push_back(newframeDelta);
        frameDelta = 20000;
        return;
    }
    else
    {
        std::vector<int64_t> temp(frameDeltas.begin()+1, frameDeltas.end());
        frameDeltas = temp;
        frameDeltas.push_back(newframeDelta);
    }
    auto const count = frameDeltas.size();
    frameDelta = std::reduce(frameDeltas.begin(), frameDeltas.end()) / count;
}
