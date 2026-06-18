#include "snazzcraft-engine/world/world.hpp"

inline bool SnazzCraft::World::VoxelCollisionInfo::Collided() const
{
    return this->CollidingVoxel && this->CollidedInChunk;
}
