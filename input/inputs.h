#ifndef GINPUTS_H
#define GINPUTS_H

#include "graphics/sdl_components.h"
#include "shared/global_shared.h"
#include "keyboard.h"
#include <QDebug>

namespace Input {
class InputEvent {
public:
    void handle( SDL_Event& event);
};
}

#endif // GINPUTS_H
