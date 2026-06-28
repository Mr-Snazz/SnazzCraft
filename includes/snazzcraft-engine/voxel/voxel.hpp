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
        uint8_t ID;

        Voxel();

        Voxel(uint8_t IID);

        ~Voxel() = default;

        inline const SnazzCraft::VoxelType& GetVoxelType() const;

        inline bool HasSide(uint8_t SideIndex) const;

        inline void ChangeSideValue(uint8_t SideIndex, bool Value);

        inline void SetAllSides();

        inline void ClearAllSides();

        inline uint8_t GetSideCount() const;

    private:
        // Bit order:
        // Void, Void, Bottom, Top, Back, Right, Left, Front
        uint8_t Sides = 0x3Fu;

    public:
        static constexpr uint32_t Size = 2u;
        static constexpr int8_t MaxLightValue = 20;
        static constexpr int8_t SunlightLightValue = 12;
        static constexpr int8_t DefaultLightValue = 1;

    private:


    };
} // SnazzCraft

#include "snazzcraft-engine/voxel/voxel.inl"