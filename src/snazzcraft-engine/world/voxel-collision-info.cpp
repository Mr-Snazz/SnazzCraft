#include "snazzcraft-engine/world/world.hpp"

SnazzCraft::World::VoxelCollisionInfo::VoxelCollisionInfo()
    : CollidingVoxel(nullptr), CollidedInChunk(nullptr), CollidingVoxelIndex(0u)
{
     
}

SnazzCraft::World::VoxelCollisionInfo::VoxelCollisionInfo(SnazzCraft::Voxel* ICollidingVoxel, SnazzCraft::Chunk* ICollidedInChunk, uint32_t ICollidingVoxelIndex)
    : CollidingVoxel(ICollidingVoxel), CollidedInChunk(ICollidedInChunk), CollidingVoxelIndex(ICollidingVoxelIndex)
{

}




