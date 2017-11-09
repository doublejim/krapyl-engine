#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QMap>
#include "graphics/sdl_components.h"


/*
 *
 *
 */

namespace Input {

class Keyboard {
public:
    QMap<SDL_Keycode, bool> m_keyStates;        // See if a keycode is currently active. Down = true, up = false.
    QMap<int, SDL_Keycode> m_keyToEvent;        // Convert an event to a keycode.
    void keyDown(SDL_Keycode &key);
    void keyUp(SDL_Keycode &key);
};
}

#endif // KEYBOARD_H
