#pragma once

#include <variant>
#include <vector>
#include <glm/glm.hpp>

namespace cup::input {
    template<InputTypes>
    class Binding {};
    
    template<>
    class Binding<InputTypes::Key> {
    private:
        void onInput(int action);
        float value_; // between 0 and 1
    };

    template<>
    class Binding<InputTypes::Axis1D> {
    private:
        Binding<InputTypes::Axis1D>& with(const char* dir, Key key);
        void up(int action);
        void down(int action);
        float value_; // between -1 and 1
    };

    template<>
    class Binding<InputTypes::Axis2D> {
    private:
        Binding<InputTypes::Axis2D>& with(const char* dir, Key key);
        void up(int action);
        void down(int action);
        void left(int action); 
        void right(int action);
        glm::vec2 value_; // between (-1, -1) and (1, 1)
    };

    typedef std::vector<std::variant<Binding<InputTypes::Key>, Binding<InputTypes::Axis1D>, Binding<InputTypes::Axis2D>>> BindingVector;

}
