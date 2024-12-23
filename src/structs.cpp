#include "structs.hpp"

// SDL render and window context
App *app = new App();
// Uint32 fps = 1;
Uint32 frameDuration = 16;

std::string GameItem::toString(void) const
{
    std::ostringstream buffer;
    buffer << position.x << "," << position.y << "," << color.r << "," << color.g << ","<< color.b;
    return buffer.str();
}

GameItem GameItem::toGameItem(const std::string& str)
{
    GameItem game_item;
    std::istringstream iss(str);
    std::string token;

    std::getline(iss, token,  ',');
    game_item.position.x = std::stof(token);

    std::getline(iss, token,  ',');
    game_item.position.y = std::stof(token);

    std::getline(iss, token,  ',');
    game_item.color.r = std::stoi(token);

    std::getline(iss, token,  ',');
    game_item.color.g = std::stoi(token);

    std::getline(iss, token);
    game_item.color.b = std::stoi(token);
    
    return game_item;
}