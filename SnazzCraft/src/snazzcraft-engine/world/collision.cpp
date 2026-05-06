#include "snazzcraft-engine/world/world.hpp"
#include "snazzcraft-engine/world/chunk.hpp"
#include "snazzcraft-engine/utilities/math.hpp"
#include "snazzcraft-engine/world/voxel-type.hpp"

SnazzCraft::World::VoxelCollisionInfo SnazzCraft::World::GetCollidingVoxel(const glm::vec3& Position, SnazzCraft::Hitbox* Hitbox, bool TestEntityCollidablility, bool TestVoxelCollidablility) const
{
    int32_t ChunkCoordinates[2];
    glm::vec3 VoxelSpacePosition = Position / glm::vec3(static_cast<float>(SnazzCraft::Voxel::Size));
    SnazzCraft::Chunk::GetChunkPosition(VoxelSpacePosition, ChunkCoordinates);

    VoxelCollisionInfo CollisionInfo;
    for (int32_t X = ChunkCoordinates[0] - 1; X <= ChunkCoordinates[0] + 1; X++) {
    for (int32_t Z = ChunkCoordinates[1] - 1; Z <= ChunkCoordinates[1] + 1; Z++) {
        if (!this->ChunkWithinWorld(X, Z)) continue;
        
        auto ChunkIterator = this->Chunks.find(SnazzCraft::IntegerHash(X, Z));
        if (ChunkIterator == this->Chunks.end()) continue;

        SnazzCraft::World::VoxelCollisionInfo VoxelCollisinoInfo = ChunkIterator->second->GetCollidingVoxel(Position, Hitbox, TestEntityCollidablility, TestVoxelCollidablility);
        if (VoxelCollisinoInfo.Collided()) return VoxelCollisinoInfo;
    }
    }

    return SnazzCraft::World::VoxelCollisionInfo();
}

SnazzCraft::World::VoxelCollisionInfo SnazzCraft::World::GetCollidingVoxel(const glm::vec3& Position, bool TestEntityCollidablility, bool TestVoxelCollidablility) const
{
    int32_t ChunkCoordinates[2];
    glm::vec3 VoxelSpacePosition = Position / glm::vec3(static_cast<float>(SnazzCraft::Voxel::Size));
    SnazzCraft::Chunk::GetChunkPosition(VoxelSpacePosition, ChunkCoordinates);

    if (!this->ChunkWithinWorld(ChunkCoordinates[0], ChunkCoordinates[1])) return SnazzCraft::World::VoxelCollisionInfo();

    auto ChunkIterator = this->Chunks.find(SnazzCraft::IntegerHash(ChunkCoordinates[0], ChunkCoordinates[1]));
    if (ChunkIterator == this->Chunks.end()) return SnazzCraft::World::VoxelCollisionInfo();

    return ChunkIterator->second->GetCollidingVoxel(Position, TestEntityCollidablility, TestVoxelCollidablility);
}