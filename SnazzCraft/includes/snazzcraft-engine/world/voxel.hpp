#pragma once

#include <vector>
#include <stdint.h>

#include "external/glm/glm.hpp"

namespace SnazzCraft
{
    class VoxelType;

    class Voxel
    {
    public:
        static constexpr uint32_t Size = 2;
        static constexpr int32_t MaxLightValue = 20;
        static constexpr int32_t SunlightLightValue = 12;

        union 
        {
            struct 
            {
                uint8_t X, Y, Z;
            };
            uint8_t Position[3]; // In local chunk space
        };
        uint8_t ID;

        Voxel(uint8_t IX, uint8_t IY, uint8_t IZ, uint8_t IID);

        ~Voxel() = default;

        const SnazzCraft::VoxelType& GetVoxelType() const;

        inline bool HasSide(uint8_t SideIndex) const;

        inline void ChangeSideValue(uint8_t SideIndex, bool Value);

        inline void SetAllSides();

        inline void ClearAllSides();

        inline uint8_t GetSideCount() const;

    private:
        // Bit order:
        // false, false, Bottom, Top, Back, Right, Left, Front
        uint8_t Sides = 0x3F;

    };
} // SnazzCraft

#include "snazzcraft-engine/world/voxel.inl"