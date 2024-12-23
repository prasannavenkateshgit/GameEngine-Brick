#include "draw.hpp"

// struct Camera {
//     SDL_Rect viewRect;  // Camera's view area (position and size)

//     Camera(int x, int y, int w, int h) {
//         viewRect = {x, y, w, h};
//     }

//     // Function to center the camera on a target (e.g., player)
//     void centerOnObject(SDL_Rect& target) {
//         // Center the camera on the object
//         viewRect.x = target.x + (target.w / 2) - (viewRect.w / 2);
//         viewRect.y = target.y + (target.h / 2) - (viewRect.h / 2);

//         // Keep the camera within the bounds of the world
//         if (viewRect.x < 0) viewRect.x = 0;
//         if (viewRect.y < 0) viewRect.y = 0;
//         if (viewRect.x > LEVEL_WIDTH - viewRect.w) viewRect.x = LEVEL_WIDTH - viewRect.w;
//         if (viewRect.y > LEVEL_HEIGHT - viewRect.h) viewRect.y = LEVEL_HEIGHT - viewRect.h;
//     }
// };

// bool recording = false;

// Camera camera(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
std::vector<RenderEvent*> rendered_events;

void prepareScene(void)
{
    for(auto ev: rendered_events)
    {
        event_manager.deregisterEvent(ev);
    }
    rendered_events.clear();
    //Sets the background to blue
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    //Clears the renderer
    SDL_RenderClear(app->renderer);
}

void presentScene(GameObjects* game_objects, std::string selfID)
{
    //draw enitites
    SDL_Rect track;

    //SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    //SDL_RenderClear(app->renderer);

    // SDL_SetRenderDrawColor(app->renderer, 96, 128, 255, 255);
    // SDL_Rect world = {-camera.viewRect.x, -camera.viewRect.y, LEVEL_WIDTH, LEVEL_HEIGHT};
    // SDL_RenderFillRect(app->renderer, &world);

    //SDL_SetRenderDrawColor(app->renderer, 255, 0, 0, 255);
    //SDL_Rect cameraBounds = {camera.viewRect.x, camera.viewRect.y, camera.viewRect.w, camera.viewRect.h};
    //SDL_RenderDrawRect(app->renderer, &cameraBounds);
    if(playback)
    {
        //event_manager.playbackEvents(camera);
    }
    else
    {
        for(auto ent: game_objects->getEntities())
        {
            //std::cout<<"Presenting Entities"<<std::endl;
            // if(!global_time->isPaused())
            // {
            //     if(ent.second->getMovable())
            //     {
            //         //ent.second->moveWithCamera(global_time->getTic(), false, camera.viewRect.x, camera.viewRect.y);
            //         ent.second->move(global_time->getTic(), false);
            //         track.x = ent.second->getX();
            //         track.y = ent.second->getY();
            //         track.w = ent.second->getW();
            //         track.h = ent.second->getH();
            //         camera.centerOnObject(track);
            //     }
            //     physics->applyGravity(ent.second, ent.first, game_objects);
            // }
            // if (ent.second->getX() + ent.second->getW() > camera.viewRect.x &&
            //     ent.second->getX() < camera.viewRect.x + camera.viewRect.w &&
            //     ent.second->getY() + ent.second->getH() > camera.viewRect.y &&
            //     ent.second->getY() < camera.viewRect.y + camera.viewRect.h)
            //     {
            //         //ent.second->drawEntity(app->renderer);
            //         ent.second->drawEntityCamera(app->renderer, camera.viewRect.x, camera.viewRect.y);
            //     }
            RenderEvent* re = new RenderEvent(ent.second, global_time->getTime(), EventPriority::HIGH);
            //re->setType(ent.first);
            event_manager.registerEvent(re);
            event_manager.raiseEvent(re);
            //event_manager.recordEvents(ent.second->toString());
            rendered_events.push_back(re);
        }
        // if (recording){
        //         event_manager.recordEvents(rendered_events);
        // }
    }
    //Render the scene 
    event_manager.handleEvents();
    SDL_RenderPresent(app->renderer);
}