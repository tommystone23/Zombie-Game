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

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    return 0;
}
