#pragma once

#include "snazzcraft-engine/world/world.hpp"

namespace SnazzCraft
{
    class SnazzCraft::World::LightNode
    {
    public:
        union 
        {
            struct 
            {
                int32_t X, Y, Z;
            };
            int32_t Position[3];
        };
        int8_t LightValue;

        LightNode(int8_t ILightValue, int32_t IX, int32_t IY, int32_t IZ);

        LightNode(int8_t ILightValue, int32_t IPosition[3]);
    };
}