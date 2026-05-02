#include "snazzcraft-engine/world/world.hpp"
#include "snazzcraft-engine/world/chunk.hpp"
#include "snazzcraft-engine/utilities/math.hpp"

SnazzCraft::Voxel* SnazzCraft::World::GetCollidingVoxel(const glm::vec3& Position, SnazzCraft::Hitbox* Hitbox) const
{
    int32_t ChunkCoordinates[2];
    glm::vec3 VoxelSpacePosition = Position / glm::vec3(static_cast<float>(SnazzCraft::Voxel::Size));
    SnazzCraft::Chunk::GetChunkPosition(VoxelSpacePosition, ChunkCoordinates);

    for (int32_t X = ChunkCoordinates[0] - 1; X <= ChunkCoordinates[0] + 1; X++) {
    for (int32_t Z = ChunkCoordinates[1] - 1; Z <= ChunkCoordinates[1] + 1; Z++) {
        if (!this->ChunkWithinWorld(X, Z)) continue;
        
        auto ChunkIterator = this->Chunks.find(SnazzCraft::IntegerHash(X, Z));
        if (ChunkIterator == this->Chunks.end()) continue;

        SnazzCraft::Voxel* CollisionVoxel = ChunkIterator->second->GetCollidingVoxel(Position, Hitbox);
        if (CollisionVoxel != nullptr) return CollisionVoxel;
    }
    }

    return nullptr;
}

SnazzCraft::Voxel* SnazzCraft::World::GetCollidingVoxel(const glm::vec3& Position) const
{
    int32_t ChunkCoordinates[2];
    glm::vec3 VoxelSpacePosition = Position / glm::vec3(static_cast<float>(SnazzCraft::Voxel::Size));
    SnazzCraft::Chunk::GetChunkPosition(VoxelSpacePosition, ChunkCoordinates);

    if (!this->ChunkWithinWorld(ChunkCoordinates[0], ChunkCoordinates[1])) return nullptr;

    auto ChunkIterator = this->Chunks.find(SnazzCraft::IntegerHash(ChunkCoordinates[0], ChunkCoordinates[1]));
    if (ChunkIterator == this->Chunks.end()) return nullptr;

    SnazzCraft::Voxel* CollisionVoxel = ChunkIterator->second->GetCollidingVoxel(Position);
    if (CollisionVoxel != nullptr) return CollisionVoxel;

    return nullptr;
}