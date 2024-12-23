#include "entity_components.hpp"

PositionComponent::PositionComponent(float x, float y)
{
    this->x = x;
    this->y = y;
    init_x = x;
    init_y = y;
}

float PositionComponent::getX()
{
    return x;
}

float PositionComponent::getY()
{
    return y;
}

float PositionComponent::getInitX()
{
    return init_x;
}

float PositionComponent::getInitY()
{
    return init_y;
}

void PositionComponent::setX(float x)
{
    this->x = x;
}

void PositionComponent::setY(float y)
{
    this->y = y;
}

void PositionComponent::scale()
{
    x *= SCREEN_WIDTH_FACTOR;
    y *= SCREEN_HEIGHT_FACTOR;
}

std::string PositionComponent::toString()
{
    return std::to_string(x) + "," + std::to_string(y);
}

void PositionComponent::fromString(const std::string& str)
{
    std::string xStr = str.substr(0, str.find(","));
    std::string yStr = str.substr(str.find(",") + 1);
    x = std::stof(xStr);
    y = std::stof(yStr);
}

SizeComponent::SizeComponent(float w, float h)
{
    this->w = w;
    this->h = h;
    init_w = w;
    init_h = h;
}

float SizeComponent::getW()
{
    return w;
}

float SizeComponent::getH()
{
    return h;
}

void SizeComponent::setW(float w)
{
    this->w = w;
}

void SizeComponent::setH(float h)
{
    this->h = h;
}

void SizeComponent::scale()
{
    w = init_w * SCREEN_WIDTH_FACTOR;
    h = init_h * SCREEN_HEIGHT_FACTOR;
}

std::string SizeComponent::toString()
{
    return std::to_string(w) + "," + std::to_string(h);
}

void SizeComponent::fromString(const std::string& str)
{
    std::string wStr = str.substr(0, str.find(","));
    std::string hStr = str.substr(str.find(",") + 1);
    w = std::stof(wStr);
    h = std::stof(hStr);
}

ColorComponent::ColorComponent(Color color)
{
    this->color = color;
    init_color = color;
}

void ColorComponent::setColor(Color color)
{
    this->color = color;
}

Color ColorComponent::getColor()
{
    return color;
}

std::string ColorComponent::toString()
{
    return std::to_string(color.r) + "," + std::to_string(color.g) + "," + std::to_string(color.b);
}

void ColorComponent::fromString(const std::string& str)
{
    std::string rStr = str.substr(0, str.find(","));
    std::string gStr = str.substr(str.find(",") + 1, str.find_last_of(","));
    std::string bStr = str.substr(str.find_last_of(",") + 1);
    color.r = std::stoi(rStr);
    color.g = std::stoi(gStr);
    color.b = std::stoi(bStr);
}

VelocityComponent::VelocityComponent(float velX, float velY)
{
    this->velX = velX;
    this->velY = velY;
    init_velX = velX;
    init_velY = velY;
}

float VelocityComponent::getVelocityX()
{
    return velX;
}

float VelocityComponent::getVelocityY()
{
    return velY;
}

void VelocityComponent::setVelocityX(float velX)
{
    this->velX = velX;
}

void VelocityComponent::setVelocityY(float velY)
{
    this->velY = velY;
}

std::string VelocityComponent::toString()
{
    return std::to_string(velX) + "," + std::to_string(velY);
}

void VelocityComponent::fromString(const std::string& str)
{
    std::string velXStr = str.substr(0, str.find(","));
    std::string velYStr = str.substr(str.find(",") + 1);
    velX = std::stof(velXStr);
    velY = std::stof(velYStr);
}
