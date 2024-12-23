#include "camera.hpp"

Camera::Camera(int x, int y, int w, int h) {
    viewRect = {x, y, w, h};
}

void Camera::centerOnObject(SDL_Rect& target) {
    // Center the camera on the object
    viewRect.x = target.x + (target.w / 2) - (viewRect.w / 2);
    viewRect.y = target.y + (target.h / 2) - (viewRect.h / 2);

    // Keep the camera within the bounds of the world
    if (viewRect.x < 0) viewRect.x = 0;
    if (viewRect.y < 0) viewRect.y = 0;
    if (viewRect.x > LEVEL_WIDTH - viewRect.w) viewRect.x = LEVEL_WIDTH - viewRect.w;
    if (viewRect.y > LEVEL_HEIGHT - viewRect.h) viewRect.y = LEVEL_HEIGHT - viewRect.h;
}

Camera camera(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);