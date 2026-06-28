#pragma once

#include <stdint.h>

namespace SnazzCraft
{
    class FluidType
    {
    public:
        enum class FluidTypeID : uint8_t
        {
            Water,
            Lava
        };

    public:
        const uint8_t Flow;         // In ticks
        const uint8_t MaximumReach; // In voxels

        static const FluidType& GetFluidType(FluidTypeID ID);

    private:
        FluidType(uint8_t IFlow, uint8_t IMaximumReach);

    
    };
}