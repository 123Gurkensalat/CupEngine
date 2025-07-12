#pragma once

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
namespace utils {
    static std::vector<char> readFile(const std::string& filename) 
    {
        std::ifstream file{filename, std::ios::ate | std::ios::binary};

        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }

        size_t size = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(size);

        file.seekg(0);
        file.read(buffer.data(), size);

        file.close();

        return buffer;
    }
}
