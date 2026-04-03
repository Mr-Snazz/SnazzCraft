#pragma once

#include "snazzcraft-engine/world/voxel.hpp"

namespace SnazzCraft
{
    class VoxelType
    {
    public:
        int32_t LightProducingLevel;
        int32_t LightPropogationDecrease;
        bool Cullable;
        bool CollidableToEntities;
        bool CollidableToVoxels;

        static const VoxelType& GetVoxelType(uint32_t VoxelID);

    private:
        VoxelType(int32_t ILightProducingLevel, int32_t ILightPropogationDecrease, bool ICullable, bool ICollidableToEntities, bool ICollidableToVoxels);
    };
} // SnazzCraft
