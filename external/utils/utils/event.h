#pragma once

#include <functional>
#include <vector>

namespace utils {
    template<typename ...Args>
    class Event {
    public:
        Event() {}
        Event(Event<Args...>&) = delete;
        void operator=(Event<Args...>&) = delete;

        void operator+=(std::function<void(Args...)> func) {
            listeners.emplace_back(func);
        }

        void Invoke(Args... args) {
            for(const auto& func : listeners) {
                func(args...);
            }
        }
    private:
        // functions that get called on invoke
        std::vector<std::function<void(Args...)>> listeners{};
        
    };
}
