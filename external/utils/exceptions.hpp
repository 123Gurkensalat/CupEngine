#pragma once


#include <stdexcept>
namespace cup {
    class not_implemented : public std::logic_error {
    public:
        not_implemented() : std::logic_error("Function not yet implemented!") {}
    };
}
