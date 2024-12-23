#pragma once
#include "defs.hpp"
#include "structs.hpp"
#include "entity_components.hpp"
#include <SDL2/SDL.h>

extern float SCREEN_WIDTH_FACTOR, SCREEN_HEIGHT_FACTOR;

class Entity
{
protected:
    PositionComponent* position;
    SizeComponent* size;
    ColorComponent* color;
    VelocityComponent* velocity;

public:
    bool underGravity;
    Entity(float x, float y, float h, float w, bool movable, Color color, bool underGravity=false);
    bool getMovable(void);
    void drawColor(SDL_Renderer* renderer);
    Coord clipCoords(Coord coord);
    virtual void drawEntity(SDL_Renderer* renderer) = 0;
    virtual void drawEntityCamera(SDL_Renderer* renderer, float cameraX, float cameraY) = 0;
    virtual void move(float deltaTime, bool absolute) = 0;
    // virtual void moveWithCamera(float deltaTime, bool absolute, float cameraX, float cameraY) = 0;
    virtual void scale() = 0;
    virtual float getX(void);
    virtual float getY(void);
    virtual float getW(void);
    virtual float getH(void);
    virtual void setX(float);
    virtual void setY(float);
    virtual void setW(float);
    virtual void setH(float);
    void setVelocityX(float vel_x);
    void setVelocityY(float vel_y);
    float getVelocityX();
    float getVelocityY();
    void setColor(Color color);
    Color getColor() const;
    std::string toString(void) const;
    void fromString(const std::string& str);
};

class Rect: public Entity{
    public:
    Rect(float x, float y, float w, float h, bool movable, Color color, bool underGravity=false);
    void move(float deltaTime, bool absolute);
    // void moveWithCamera(float deltaTime, bool absolute, float cameraX, float cameraY);
    void scale();
    void drawEntity(SDL_Renderer* renderer);
    void drawEntityCamera(SDL_Renderer* renderer, float cameraX, float cameraY);
    float getX(void);
    float getY(void);
    float getW(void);
    float getH(void);
    void setX(float);
    void setY(float);
    void setW(float);
    void setH(float);
    SDL_Rect getRect(void);
    std::string toString(void) const;
    void fromString(const std::string& str);
    private:
    bool movable;
    SDL_Rect rect;
};
