#include "keyboard.h"
namespace Input {
void Keyboard::keyDown(SDL_Keycode& key)
{
    m_keyStates.insert( key, true);
}

void Keyboard::keyUp(SDL_Keycode &key)
{
    m_keyStates.insert( key, false);
}
}
