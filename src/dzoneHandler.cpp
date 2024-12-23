#include "dzoneHandler.hpp"

// Definition of the static member variable
DzoneHandler* DzoneHandler::instance = nullptr;

// Add a death zone with the corresponding spawn point
void DzoneHandler::addDeathZone(const SDL_Rect& zone, const Coord& spawn) {
    dzones.push_back(zone);
    spawn_points.push_back(spawn);
}

// Get the count of death zones
size_t DzoneHandler::getZoneCount() const {
    return dzones.size();
}

// Get a specific death zone by index
SDL_Rect DzoneHandler::getDeathZone(size_t index) const {
    if (index < dzones.size()) {
        return dzones[index];
    }
    throw std::out_of_range("Index out of range for death zones");
}

// Get a specific spawn point by index
Coord DzoneHandler::getSpawnPoint(size_t index) const {
    if (index < spawn_points.size()) {
        return spawn_points[index];
    }
    throw std::out_of_range("Index out of range for spawn points");
}

// Update a death zone and its spawn point by index
void DzoneHandler::updateDeathZone(size_t index, const SDL_Rect& newZone, const Coord& newSpawn) {
    if (index < dzones.size()) {
        dzones[index] = newZone;
        spawn_points[index] = newSpawn;
    } else {
        throw std::out_of_range("Index out of range for death zones");
    }
}

// Remove a death zone and its spawn point by index
void DzoneHandler::removeDeathZone(size_t index) {
    if (index < dzones.size()) {
        dzones.erase(dzones.begin() + index);
        spawn_points.erase(spawn_points.begin() + index);
    } else {
        throw std::out_of_range("Index out of range for death zones");
    }
}

// Start collision checking in a separate thread
void DzoneHandler::startCollisionChecking(Rect* playerE) {
    this->playerE = playerE;
    if (running) {
        std::cout << "Collision checking is already running." << std::endl;
        return;
    }
    running = true;
    collisionThread = std::thread(&DzoneHandler::checkPlayerCollisionLoop, this);
}

// Stop the collision checking
void DzoneHandler::stopCollisionChecking() {
    running = false;
    if (collisionThread.joinable()) {
        collisionThread.join();
    }
}

// Collision checking loop
void DzoneHandler::checkPlayerCollisionLoop() {
    while (running) { // Run while the checking is active
        SDL_Rect player = playerE->getRect();
        for (size_t i = 0; i < dzones.size(); ++i) {
            if (checkCollision(player, dzones[i])) {
                Coord spawn_point = spawn_points[i];
                DeathZoneEvent* dEvent = new DeathZoneEvent(spawn_point, playerE, global_time->getTime(), EventPriority::HIGH);
                dEvent->setType("DZONE");
                event_manager.registerEvent(dEvent);
                event_manager.raiseEvent(dEvent);
                
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Sleep to prevent high CPU usage
    }
}

// Check for collision between player and death zone
bool DzoneHandler::checkCollision(const SDL_Rect& player, const SDL_Rect& dzone) {
    return SDL_HasIntersection(&player, &dzone);
}
