#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

namespace cup::ecs 
{
    class IComponentArray {
    public:
        IComponentArray() = default;
        IComponentArray(unsigned int size) : size_(size) {}
        IComponentArray(unsigned int size, unsigned int capacity) : size_(size), capacity_(capacity) {}

        virtual std::unique_ptr<IComponentArray> createNewArray() = 0;
        virtual void push_back(void* data) = 0;
        virtual void pop_at(uint32_t i) = 0; 
        virtual void* get(uint32_t i) = 0;
    protected:
        uint32_t size_{0};
        uint32_t capacity_{0};
    };

    template<typename T>
    class ComponentArray : public IComponentArray{
    public:
        ComponentArray() : IComponentArray() {}
        ComponentArray(unsigned int size) : IComponentArray(size) { reserve(size); }

        ComponentArray(const ComponentArray<T>&) = delete;
        T& operator=(const ComponentArray<T>&) = delete;
        
        ComponentArray(ComponentArray<T>&& other) noexcept;

        T& operator=(ComponentArray<T>&& other) noexcept;
            
        ~ComponentArray() { delete[] buffer_; }

        std::unique_ptr<IComponentArray> createNewArray();

        void push_back(void* data);

        void pop_at(uint32_t i);
            
        void* get(uint32_t i) { return &buffer_[i]; }

    private:
        void reserve(size_t capacity);

        T* buffer_ = nullptr;
    };

    template<typename T>
    ComponentArray<T>::ComponentArray(ComponentArray<T>&& other) noexcept 
        : buffer_(other.buffer_), IComponentArray(other.size_, other.capacity_)
    {
        other.buffer_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    template<typename T>
    T& ComponentArray<T>::operator=(ComponentArray<T>&& other) noexcept 
    {
        if (&other == this) {
            return *this;
        }

        delete[] buffer_;

        buffer_ = other.buffer_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.buffer_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;

        return *this;
    }

    template<typename T>
    std::unique_ptr<IComponentArray> ComponentArray<T>::createNewArray() 
    {
        std::unique_ptr<ComponentArray<T>> ptr{new ComponentArray<T>()};
        return ptr;
    }

    template<typename T>
    void ComponentArray<T>::push_back(void* data) 
    {
        if (size_ >= capacity_) 
            reserve(capacity_ + 5);
        buffer_[size_++] = std::move(*static_cast<T*>(data));
    }


    template<typename T>
    void ComponentArray<T>::pop_at(uint32_t i) 
    {
        buffer_[i] = std::move(buffer_[size_ - 1]);
        size_--;
    }


    template<typename T>
    void ComponentArray<T>::reserve(size_t capacity) 
    {
        T* newBuffer = new T[capacity];

        uint32_t minSize = capacity < size_ ? capacity : size_;

        for (uint32_t i = 0; i < minSize; i++)
            newBuffer[i] = buffer_[i];

        capacity_ = capacity;
        delete[] buffer_;
        buffer_ = newBuffer;
    }
}
