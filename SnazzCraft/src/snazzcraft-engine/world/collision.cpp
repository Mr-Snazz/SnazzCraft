#include "snazzcraft-engine/world/world.hpp"
#include "snazzcraft-engine/world/chunk.hpp"

SnazzCraft::Voxel* SnazzCraft::World::GetCollidingVoxel(const glm::vec3& Position, SnazzCraft::Hitbox* Hitbox) const
{
    const int32_t ChunkX = static_cast<int32_t>(Position.x / (SnazzCraft::Chunk::Width * SnazzCraft::Voxel::Size));
    const int32_t ChunkZ = static_cast<int32_t>(Position.z / (SnazzCraft::Chunk::Depth * SnazzCraft::Voxel::Size));

    for (int32_t X = ChunkX - 1; X <= ChunkX + 1; X++) {
    for (int32_t Z = ChunkZ - 1; Z <= ChunkZ + 1; Z++) {
        if (X < 0 || Z < 0 || X >= this->Size || Z >= this->Size) continue;
        
        auto ChunkIterator = this->Chunks.find(SnazzCraft::Index2D<int32_t>(X, Z, this->Size));
        if (ChunkIterator == this->Chunks.end()) continue;

        SnazzCraft::Voxel* CollisionVoxel = ChunkIterator->second->GetCollidingVoxel(Position, Hitbox);
        if (CollisionVoxel != nullptr) return CollisionVoxel;
    }
    }

    return nullptr;
}

SnazzCraft::Voxel* SnazzCraft::World::GetCollidingVoxel(const glm::vec3& Position) const
{
    const int32_t ChunkX = static_cast<int32_t>(Position.x / (SnazzCraft::Chunk::Width * SnazzCraft::Voxel::Size));
    const int32_t ChunkZ = static_cast<int32_t>(Position.z / (SnazzCraft::Chunk::Depth * SnazzCraft::Voxel::Size));

    if (ChunkX < 0 || ChunkZ < 0 || ChunkX >= this->Size || ChunkZ >= this->Size) return nullptr;

    auto ChunkIterator = this->Chunks.find(SnazzCraft::Index2D<int32_t>(ChunkX, ChunkZ, this->Size));
    if (ChunkIterator == this->Chunks.end()) return nullptr;

    SnazzCraft::Voxel* CollisionVoxel = ChunkIterator->second->GetCollidingVoxel(Position);
    if (CollisionVoxel != nullptr) return CollisionVoxel;

    return nullptr;
}