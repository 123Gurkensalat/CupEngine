#pragma once

#include <memory>

namespace utils {

    template<typename T>
    class Singleton {
    public:
        Singleton(Singleton&) = delete;
        void operator=(const Singleton&) = delete;
        
    protected:
        static T& instance() {
            if (!instance_) {
                instance_ = std::make_unique<T>();
            }
            return *instance_.get();
        }

    private:
        static std::unique_ptr<T> instance_;
    };

    template<typename T>
    std::unique_ptr<T> Singleton<T>::instance_{};
}
