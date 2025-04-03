#pragma once

#import "action.hpp"
#import "action_map.hpp"

namespace cup::input {
////  Key  ////////////////////////////////////////////////////////////////////

    template<InputDevice>
    void Action<InputType::Key>::addBinding(KeyCode key) 
    {
        actionMap.keyboardEvents.at(key) += [&](int action){onKeyEvent(action);};         
    }

    template<InputDevice>
    void Action<InputType::Key>::changeBinding(KeyCode old, KeyCode key) 
    {

    }
    
    template<InputDevice>
    void Action<InputType::Key>::deleteBinding(KeyCode key) 
    {

    }

////  Axis 1D  ////////////////////////////////////////////////////////////////
////  Axis 2D  ////////////////////////////////////////////////////////////////
}
