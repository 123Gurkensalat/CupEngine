#pragma once

#include <functional>
#include <vector>

namespace utils {
    template<typename ...Args>
    class Event {
    public:
        using Handler = std::function<void(Args...)>;
        Event() {}
        Event(Event<Args...>&) = delete;
        void operator=(Event<Args...>&) = delete;

        void operator+= (const Handler& func) {
            handlers.emplace_back(func);
        }

        void Invoke(Args... args) {
            for(const auto& func : handlers) {
                func(args...);
            }
        }
    private:
        // functions that get called on invoke
        std::vector<Handler> handlers{};
        
    };
}
