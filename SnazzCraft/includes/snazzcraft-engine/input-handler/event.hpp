#pragma once

#include <stdint.h>

#include "glfw3.h"

#include "data.hpp"

namespace SnazzCraft
{
    class Data;

    class Event
    {
    public:
        SnazzCraft::Data* EventData;
        uint8_t Type;

        Event(uint8_t IType);

        virtual ~Event();

    private:


    };
} // SnazzCraft

