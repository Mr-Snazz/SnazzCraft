#include "snazzcraft-engine/world/world.hpp"
#include "snazzcraft-engine/world/chunk.hpp"

inline void SnazzCraft::World::ApplyGravityToAllEntities() const
{
    constexpr float MoveDistance = 0.2f;

    this->MoveEntity(glm::vec3(0.0f, -MoveDistance, 0.0f), SnazzCraft::Player);
    for (SnazzCraft::Entity* Entity : this->Entities) {
        this->MoveEntity(glm::vec3(0.0f, -MoveDistance, 0.0f), Entity);
    }
}

inline const glm::vec3& SnazzCraft::World::GetVoxelPlacementDisplayPosition() const
{
    static const glm::vec3 DefaultReturnPosition(-100.0f, -100.0f, -100.0f);
    return this->ShouldRenderVoxelPlacementDisplay ? this->VoxelPlacementDisplayPosition : DefaultReturnPosition;
}

inline bool SnazzCraft::World::ChunkWithinWorld(SnazzCraft::Chunk* Chunk) const
{
    return Chunk != nullptr && Chunk->Position[0] >= 0 && Chunk->Position[0] < this->Size && Chunk->Position[1] >= 0 && Chunk->Position[1] < this->Size;
}

inline bool SnazzCraft::World::ChunkWithinWorld(int32_t ChunkX, int32_t ChunkZ) const
{
    return ChunkX >= 0 && ChunkX < this->Size && ChunkZ >= 0 && ChunkZ < this->Size;
}


