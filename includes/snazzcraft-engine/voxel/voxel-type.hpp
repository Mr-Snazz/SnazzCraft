#pragma once

#include <stdint.h>

namespace SnazzCraft
{
    class VoxelType
    {
    public:
        enum class VoxelTypeID : uint8_t
        {
            Stone,
            OakWood,
            Grass,
            Dirt,
            AboveGrass,
            DirtGrassMix,
            OakLog,
            OakLeaves,
            CoalOre,
            IronOre,
            GoldOre,
            RedstoneOre,
            DiamonOre,
            EmeraldOre,
            Barrier,
            Torch,
            Water,
            RedstoneWireStraight,
            Sand,
            Air
        };

    public:
        const int8_t LightProducingLevel;
        const int8_t LightPropogationDecrease;
        const uint8_t CullableSides;
        const bool CollidableToEntities;
        const bool CollidableToVoxels;
        const bool CullableAgainst;

        /*
        Bit order of CullableSides:
            Void, Void, Bottom, Top, Back, Right, Left, Front
        */

        static const VoxelType& GetVoxelType(VoxelTypeID VoxelID);

    private:
        VoxelType(int8_t ILightProducingLevel, int8_t ILightPropogationDecrease, uint8_t ICullableSides, bool ICollidableToEntities, bool ICollidableToVoxels, bool ICullableAgainst);
    };
} // SnazzCraft
