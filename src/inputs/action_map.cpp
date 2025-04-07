#include "action_map.hpp"

using namespace cup::input;

void ActionMap::key_callback(KeyCode key, int scancode, int action, int mods) 
{
    if (keyboard_events.find(key) == keyboard_events.end()) return;
    keyboard_events.at(key).Invoke(key, action);
}
