#pragma once
#include "defs.hpp"
#include "structs.hpp"
#include <SDL2/SDL.h>

extern float SCREEN_WIDTH_FACTOR, SCREEN_HEIGHT_FACTOR;

class BaseComponent
{
public:
    virtual ~BaseComponent() = default;
    virtual std::string toString() = 0;
    virtual void fromString(const std::string& str) = 0;
};

class PositionComponent : public BaseComponent
{
private:
    float init_x, init_y, x, y;
public:
    PositionComponent(float x, float y);
    float getX();
    float getY();
    float getInitX();
    float getInitY();
    void setX(float x);
    void setY(float y);
    void scale();
    std::string toString();
    void fromString(const std::string& str);
};

class SizeComponent : public BaseComponent
{
private:
    float init_w, init_h, w, h;
public:
    SizeComponent(float w, float h);
    float getW();
    float getH();
    void setW(float w);
    void setH(float h);
    void scale();
    std::string toString();
    void fromString(const std::string& str);
};

class ColorComponent : public BaseComponent
{
private:
    Color init_color, color;
public:
    ColorComponent(Color color);
    void setColor(Color color);
    Color getColor();
    std::string toString();
    void fromString(const std::string& str);
};

class VelocityComponent : public BaseComponent
{
private:
    float init_velX, init_velY, velX, velY;
public:
    VelocityComponent(float velX, float velY);
    float getVelocityX();
    float getVelocityY();
    void setVelocityX(float velX);
    void setVelocityY(float velY);
    std::string toString();
    void fromString(const std::string& str);
};
