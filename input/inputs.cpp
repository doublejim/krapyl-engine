#include "inputs.h"
namespace Input {
void InputEvent::handle(SDL_Event &event)
{
    switch (event.type)
    {
    case SDL_WINDOWEVENT:
    {
        switch(event.window.event)
        {
        case SDL_WINDOWEVENT_CLOSE:
            Shared::global.closeProgram = true;
            qDebug() << "sdl windowevent close!";
            break;
        }
        break;
    }
    case SDL_MOUSEMOTION:
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        Shared::global.mouse.mouseMove( x, y);
        break;
    }
    case SDL_MOUSEBUTTONDOWN:
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        break;
    } 
    case SDL_MOUSEBUTTONUP:
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        break;
    }

    case SDL_KEYDOWN:
    {
        Shared::global.keyboard.keyDown( event.key.keysym.sym);
        break;
    }
    case SDL_KEYUP:
    {
        Shared::global.keyboard.keyUp( event.key.keysym.sym);
        break;
    }
    case SDL_QUIT:
    {
        Shared::global.closeProgram = true;
        qDebug() << "sdl QUIT!";
        break;
    }
    }
}
}
