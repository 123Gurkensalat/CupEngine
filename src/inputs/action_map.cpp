#include "action_map.hpp"

using namespace cup::input;

void ActionMap::key_callback(KeyCode key, int scancode, int action, int mods) 
{
    key_events[key].Invoke(key, action);
}
