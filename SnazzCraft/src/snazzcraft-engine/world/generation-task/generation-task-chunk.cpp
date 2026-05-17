#include "snazzcraft-engine/world/generation-task/generation-task-chunk.hpp"
#include "snazzcraft-engine/world/generation-task/generation-task-ids.h"

SnazzCraft::World::GenerationTaskChunk::GenerationTaskChunk(SnazzCraft::World* IWorld, int32_t IChunkX, int32_t IChunkZ)
    : GenerationTask(GENERATION_TASK_ID_CHUNK, IWorld), ChunkX(IChunkX), ChunkZ(IChunkZ)
{

}

void SnazzCraft::World::GenerationTaskChunk::Execute() const
{
    this->World->GenerateChunk(this->ChunkX, this->ChunkZ, true);
}
