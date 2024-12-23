#pragma once

#include <vector>
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include "entity.hpp"
#include "event.hpp"
#include "event_manager.hpp"

extern std::string selfID;

class DzoneHandler {
public:
    void addDeathZone(const SDL_Rect& zone, const Coord& spawn);
    void updateDeathZone(size_t index, const SDL_Rect& newZone, const Coord& newSpawn);
    void removeDeathZone(size_t index);
    size_t getZoneCount() const;
    SDL_Rect getDeathZone(size_t index) const;
    Coord getSpawnPoint(size_t index) const;

    void startCollisionChecking(Rect* playerE);
    void stopCollisionChecking();

    static DzoneHandler& getInstance() {
        if (!instance) {
            instance = new DzoneHandler(); // Create the instance if it doesn't exist
        }
        return *instance;
    }

private:
    static DzoneHandler* instance; // Pointer to the single instance
    std::vector<Coord> spawn_points;
    std::vector<SDL_Rect> dzones;
    Rect* playerE;
    std::atomic<bool> running{ false };
    std::thread collisionThread;

    DzoneHandler() = default; // Private constructor
    DzoneHandler(const DzoneHandler&) = delete; // Delete copy constructor
    DzoneHandler& operator=(const DzoneHandler&) = delete; // Delete assignment operator

    void checkPlayerCollisionLoop();
    bool checkCollision(const SDL_Rect& player, const SDL_Rect& dzone);
};
