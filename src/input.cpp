#include "input.hpp"


/*
* This function is boilerplate, and the contents currently read the events thrown by SDL and look for the SDL quit event, terminating the program.
* This file is where code to read the keyboard state and determine which keys are pressed should sit.
*/
// float previous_key_press = 0.0f;
int previous_key_presses[SDL_NUM_SCANCODES] = {0};
bool isDisconnected = false;

InputEvent* input_event;

/**
 * This function is responsible for reading the state of the keyboard and determining if the window size has changed.
 * It calls the moveFunc function with the current state of the keyboard, and scales all entities in the map if the
 * window size has changed and isProportional is true.
 * If the 't' key is pressed, it toggles the isProportional flag and prints a message to the console.
 * If the window is closed, it terminates the program.
 */
void doInput(GameObjects* game_objects)
{
    // Uint8 const *keys;
    // bool valid_keys[SDL_NUM_SCANCODES] = {false};
    // keys = SDL_GetKeyboardState(nullptr);
    // float current_time = global_time->getTime();
    // if(global_time->isPaused())
    // {
    //     current_time += global_time->getPausedTime();
    // }
    // for (size_t i = 0; i < SDL_NUM_SCANCODES; i++)
    // {
    //     if(keys[i] == 1 && current_time - previous_key_presses[i] >= KEY_REPEAT_DELAY*frameDelta)
    //     {
    //         valid_keys[i] = true;
    //         previous_key_presses[i] = current_time;
    //     }
    // }
    
    // moveFunc(game_objects, valid_keys);
    event_manager.raiseEvent(input_event);
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                SCREEN_WIDTH_FACTOR = float(event.window.data1) / SCREEN_WIDTH;
                SCREEN_HEIGHT_FACTOR = float(event.window.data2) / SCREEN_HEIGHT;
                if(isProportional)
                {
                    game_objects->scaleAllEntities();
                }
            }
            break;
        case SDL_QUIT:
            // exit(0);
            // isDisconnected = true;
            event_manager.raiseEvent(disconnection_event);
            break;

        default:
            break;
        }
    }
}