#pragma once

#include "snazzcraft-engine/world/world.hpp"



class SnazzCraft::World::VoxelCollisionInfo
{
public:
    SnazzCraft::Voxel* CollidingVoxel;
    SnazzCraft::Chunk* CollidedInChunk;
    uint32_t CollidingVoxelIndex;

    VoxelCollisionInfo();

    VoxelCollisionInfo(SnazzCraft::Voxel* ICollidingVoxel, SnazzCraft::Chunk* ICollidedInChunk, uint32_t ICollidingVoxelIndex);

    inline bool Collided() const;
};

#include "snazzcraft-engine/world/voxel-collision-info.inl"

