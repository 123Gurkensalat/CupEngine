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
        Event& operator=(Event<Args...>&) = delete;
        Event(Event<Args...>&&) = default;
        Event& operator=(Event<Args...>&&) = default;

        void operator+= (const Handler& func) const {
            handlers.emplace_back(func);
        }

        void Invoke(Args... args) const {
            for(const auto& func : handlers) {
                func(args...);
            }
        }
    private:
        // functions that get called on invoke
        std::vector<Handler> handlers{};
        
    };
}
