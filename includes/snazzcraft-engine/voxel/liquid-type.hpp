#pragma once

#include <stdint.h>

namespace SnazzCraft
{
    class LiquidType
    {
    public:
        enum class LiquidTypeID : uint8_t
        {
            Water,
            Lava
        };

        static constexpr uint8_t VoxelCheckPositionsIndexes[5u] = {
            0u,
            1u,
            2u,
            3u,
            // 4u, Top is not included
            5u
        };

    public:
        const uint8_t Flow;         // In ticks
        const uint8_t MaximumReach; // In voxels

        static const LiquidType& GetFluidType(LiquidTypeID ID);

    private:
        LiquidType(uint8_t IFlow, uint8_t IMaximumReach);

    
    };
}