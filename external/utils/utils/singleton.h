#pragma once

#include <memory>

namespace utils {

    template<typename T>
    class Singleton {
    public:
        Singleton(Singleton&) = delete;
        void operator=(const Singleton&) = delete;
        
    protected:
        static T* GetInstance() {
            if (!instance) {
                instance = std::make_unique<T>();
            }
            return instance.get();
        }

    private:
        static std::unique_ptr<T> instance;
    };

    template<typename T>
    std::unique_ptr<T> Singleton<T>::instance{};
}
