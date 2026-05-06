#include "snazzcraft-engine/world/world.hpp"

inline bool SnazzCraft::World::VoxelCollisionInfo::Collided() const
{
    return this->CollidingVoxel != nullptr && this->CollidedInChunk != nullptr;
}
