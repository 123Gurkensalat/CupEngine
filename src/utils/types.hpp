#pragma once

#include <cstring>
#include <functional>
#include <stdexcept>
#include <unordered_set>
#include <vulkan/vulkan.h>

namespace cup {

inline bool cstr_equal(const char* a, const char* b)
{
    return strcmp(a, b) == 0;
}

typedef std::unordered_set<const char*, std::hash<const char*>, decltype(&cstr_equal)>
    unordered_cstr_set;

class not_implemented : public std::logic_error {
public:
    not_implemented()
        : std::logic_error("Function not yet implemented!")
    {}
};

class NonCopyable {
protected:
    NonCopyable()  = default;
    ~NonCopyable() = default;

    NonCopyable(const NonCopyable&)           = delete;
    NonCopyable& operator=(const NonCopyable) = delete;
};
class NonMovable {
protected:
    NonMovable()  = default;
    ~NonMovable() = default;

    NonMovable(NonMovable&&)            = delete;
    NonMovable& operator=(NonMovable&&) = delete;
};

} // namespace cup
