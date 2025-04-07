#pragma once

#include <cstdint>
#include <unordered_map>

namespace utils {
    template<typename ...Args>
    class Event {
    public:
        Event() {}
        Event(Event<Args...>&) = delete;
        Event(const Event<Args...>&) = delete;
        Event& operator=(Event<Args...>&) = delete;
        Event& operator=(const Event<Args...>&) = delete;
        Event(Event<Args...>&&) = default;
        Event& operator=(Event<Args...>&&) = default;

        template<typename T>
        uint32_t Subscribe(T* ref, void(T::*func)(Args...));
        uint32_t Subscribe(void(*func)(Args...)) {
            basic_handlers.emplace(counter, func);
            return counter++;
        }

        void Invoke(Args... args) const {
            for (auto [_, handler] : handlers) {
                handler->call(args...);
            }
            for (auto [_, handler] : basic_handlers) {
                handler(args...);
            }
        }

        void Unscubscribe(uint32_t id) {
            handlers.erase(id);
            basic_handlers.erase(id);
        }
    private:
        struct IHandler {
            virtual void call(Args...) const {}
            protected:
            IHandler() {}
        };

        template<typename  T>
        struct Handler : public IHandler{
            Handler(T& ref, void (T::*func)(Args...)) : ref(ref), func(func) {}
            T& ref; 
            void (T::*func)(Args...);
            void call(Args...) const override;
        };

        // functions that get called on invoke
        std::unordered_map<uint32_t, IHandler*> handlers;
        std::unordered_map<uint32_t, void (*)(Args...)> basic_handlers;
        uint32_t counter{0};
    };
    
    template<typename... Args>
    template<typename T>
    void Event<Args...>::Handler<T>::call(Args... args) const {
        (ref.*func)(args...);
    }

    template<typename... Args>
    template<typename T>
    uint32_t Event<Args...>::Subscribe(T* ref, void(T::*func)(Args...)) 
    {
        auto handler = new Handler<T>{*ref, func};
        handlers.emplace(counter, handler);
        return counter++;
    }
}
