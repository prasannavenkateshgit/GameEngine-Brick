#include "entity.hpp"
#include <iostream>
using namespace std;

/**
 * Creates an Entity object with the given properties.
 *
 * @param x float The entity's x position.
 * @param y float The entity's y position.
 * @param movable bool Whether the entity is movable.
 * @param color Color The entity's color.
 * @param underGravity bool Whether the entity is affected by gravity.
 */

Entity::Entity(float x, float y, float w, float h, bool movable, Color color, bool underGravity)
{
    this->position = new PositionComponent(x, y);
    this->size = new SizeComponent(w, h);
    this->color = new ColorComponent(color);
    if(movable or underGravity)
    {
        this->velocity = new VelocityComponent(0.0f, 0.0f);
    }
    else
    {
        this->velocity = nullptr;
    }
    this->underGravity = underGravity;

}

bool Entity::getMovable(void)
{
    return this->velocity != nullptr;
}

/**
 * Set the draw color for the renderer to the entity's color.
 *
 * @param renderer The SDL_Renderer object.
 */
void Entity::drawColor(SDL_Renderer *renderer)
{
    Color c = this->color->getColor();
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
}

/**
 * Clips a given coordinate to the screen dimensions.
 *
 * @param coord The Coord object to be clipped.
 * @return The clipped Coord object.
 */
Coord Entity::clipCoords(Coord coord)
{
    coord.x = coord.x + getW() > SCREEN_WIDTH_FACTOR * SCREEN_WIDTH ? SCREEN_WIDTH_FACTOR * SCREEN_WIDTH - getW() : coord.x < 0 ? 0 : coord.x;
    coord.y = coord.y + getH() > SCREEN_HEIGHT_FACTOR * SCREEN_HEIGHT ? SCREEN_HEIGHT_FACTOR * SCREEN_HEIGHT - getH() : coord.y < 0 ? 0 : coord.y;
    return coord;
}

float Entity::getX(void)
{
    return this->position->getInitX();
}


float Entity::getY(void)
{
    return this->position->getInitY();
}

float Entity::getW(void)
{
    return this->size->getW();
}

float Entity::getH(void)
{
    return this->size->getH();
}
void Entity::setX(float x)
{
    this->position->setX(x);
}

void Entity::setY(float y)
{
    this->position->setY(y);
}

void Entity::setW(float w)
{
    this->size->setW(50);
}

void Entity::setH(float h)
{
    this->size->setH(50);
}

/**
 * Sets the horizontal velocity component of the entity.
 *
 * @param vel_x The horizontal velocity component.
 */
void Entity::setVelocityX(float vel_x)
{
    if(this->velocity)
    {
        this->velocity->setVelocityX(vel_x);
    }
}

/**
 * Sets the vertical velocity component of the entity.
 *
 * @param vel_y The vertical velocity component.
 */
void Entity::setVelocityY(float vel_y)
{
    if(this->velocity)
    {
        this->velocity->setVelocityY(vel_y);
    }
}

float Entity::getVelocityX() {
    return this->velocity?this->velocity->getVelocityX(): 0.0f;
}

float Entity::getVelocityY() {
    return this->velocity?this->velocity->getVelocityY(): 0.0f;
}

/**
 * Creates a Rect object with the given properties.
 *
 * @param x The x position of the Rect.
 * @param y The y position of the Rect.
 * @param w The width of the Rect.
 * @param h The height of the Rect.
 * @param movable Whether the Rect is movable.
 * @param color The color of the Rect.
 * @param underGravity Whether the Rect is affected by gravity.
 */
void Entity::setColor(Color color)
{
    this->color->setColor(color);
}

Color Entity::getColor() const
{
    return this->color->getColor();
}

std::string Entity::toString(void) const
{
    std::ostringstream buffer;
    buffer << this->position->toString() << "," << this->size->toString() << "," << this->color->toString();
    return buffer.str();
}

void Entity::fromString(const std::string &str)
{
    std::istringstream iss(str);
    std::string token;
    std::getline(iss, token,  ',');
    float x = std::stof(token);
    std::getline(iss, token,  ',');
    float y = std::stof(token);
    std::getline(iss, token,  ',');
    float w = std::stof(token);
    std::getline(iss, token,  ',');
    float h = std::stof(token);
    std::getline(iss, token,  ',');
    Color color;
    color.r = std::stoi(token);
    std::getline(iss, token,  ',');
    color.g = std::stoi(token);
    std::getline(iss, token);
    color.b = std::stoi(token);
    this->setX(x);
    this->setY(y);
    this->setW(w);
    this->setH(h);
    this->setColor(color);
}

Rect::Rect(float x, float y, float w, float h, bool movable, Color color, bool underGravity)
:Entity(x, y, w, h, movable, color, underGravity)
{
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    
}

/**
 * Moves the Rect by the given amount.
 *
 * @param delta_x The amount to move in the x direction.
 * @param delta_y The amount to move in the y direction.
 * @param absolute Whether to move relative to the current position or to move to an absolute position.
 */
void Rect::move(float deltaTime, bool absolute)
{
        float new_x = 0.0f, new_y = 0.0f;
        auto velX = this->getVelocityX();
        auto velY = this->getVelocityY();
        float delta_x = velX * deltaTime;
        float delta_y = velY * deltaTime;
        if (absolute)
        {
            auto x = this->position->getInitX();
            auto y = this->position->getInitY();
            new_x = x + delta_x;
            new_y = y + delta_y;
        }
        else
        {
            new_x = this->getX() + delta_x;
            new_y = this->getY() + delta_y;
        }
        Coord new_coords = Entity::clipCoords(Coord{new_x, new_y});
        if (new_x < 0) new_x = 0;
        if (new_y < 0) new_y = 0;
        if (new_x > LEVEL_WIDTH - this->getW()) new_x = LEVEL_WIDTH - this->getW();
        if (new_y > SCREEN_HEIGHT - this->getH()) new_y = SCREEN_HEIGHT - this->getH();
        this->setX(new_x);
        this->setY(new_y);
    // }
}

/**
 * Scales the Rect according to the current screen width and height factors.
 */
void Rect::scale()
{
    
    // this->position->scale();
    this->size->scale();
    // rect.x = getX();
    // rect.y = getY();
    rect.w = getW();
    rect.h = getH();
}

/**
 * Draws the Rect to the given SDL_Renderer.
 *
 * The Rect is filled with the current color, and then rendered to the
 * given SDL_Renderer.
 *
 * @param renderer The SDL_Renderer to draw to.
 */
void Rect::drawEntity(SDL_Renderer* renderer)
{
    Entity::drawColor(renderer);
    SDL_RenderFillRect(renderer, &rect);
}

void Rect::drawEntityCamera(SDL_Renderer* renderer, float cameraX, float cameraY)
{
    SDL_Rect renderobject = {rect.x - cameraX, rect.y - cameraY, rect.w, rect.h};
    //rect.x = rect.x - cameraX;
    //rect.y = rect.y - cameraY;
    Entity::drawColor(renderer);
    SDL_RenderFillRect(renderer, &renderobject);
}

float Rect::getX(void)
{
    return this->position->getX();
}

float Rect::getY(void)
{
    return this->position->getY();
}

void Rect::setX(float x)
{
    this->position->setX(x);
    rect.x = x;
}

void Rect::setY(float y)
{
    this->position->setY(y);
    rect.y = y;
}

void Rect::setW(float w)
{
    this->size->setW(w);
    rect.w = w;
}

void Rect::setH(float h)
{
    this->size->setH(h);
    rect.h = h;
}

float Rect::getW(void)
{
    return this->size->getW();
}

float Rect::getH(void)
{
    return this->size->getH();
}

SDL_Rect Rect::getRect(){
    SDL_Rect r;
    r.x = rect.x;
    r.y = rect.y;
    r.w = rect.w;
    r.h = rect.h;
    return r;
}
