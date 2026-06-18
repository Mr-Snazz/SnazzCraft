#pragma once

#include <vector>
#include <string>
#include <stdint.h>

#include "external/glm/glm.hpp"

namespace SnazzCraft
{
    struct Data
    {
    public:
        std::vector<void*> Items;
        std::vector<uint8_t> Types;

        ~Data();

        void* AccessDataType(uint8_t Type); // Returns first occurrence of data with specified type

    private:


    };
} // SnazzCraft
