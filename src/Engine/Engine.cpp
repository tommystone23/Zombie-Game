#include "Engine.h"
#include <SDL2/SDL.h>
#include "Errors.h"

int Engine::init()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        fatal_error("Failed to initialize SDL");
        return 0;
    }
    return 0;
}
