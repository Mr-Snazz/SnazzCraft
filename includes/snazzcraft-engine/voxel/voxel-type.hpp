#pragma once

#include <stdint.h>

namespace SnazzCraft
{
    class VoxelType
    {
    public:
        int8_t LightProducingLevel;
        int8_t LightPropogationDecrease;
        uint8_t CullableSides;
        bool CollidableToEntities;
        bool CollidableToVoxels;
        bool CullableAgainst;

        /*
        Bit order of CullableSides:
            false, false, Bottom, Top, Back, Right, Left, Front
        */

        static const VoxelType& GetVoxelType(uint8_t VoxelID);

    private:
        VoxelType(int8_t ILightProducingLevel, int8_t ILightPropogationDecrease, uint8_t ICullableSides, bool ICollidableToEntities, bool ICollidableToVoxels, bool ICullableAgainst);
    };
} // SnazzCraft
