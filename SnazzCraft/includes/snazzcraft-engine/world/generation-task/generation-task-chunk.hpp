#pragma once

#include "snazzcraft-engine/world/generation-task/generation-task.hpp"


class SnazzCraft::World::GenerationTaskChunk : public SnazzCraft::World::GenerationTask
{
public:
    const int32_t ChunkX, ChunkZ;

    GenerationTaskChunk(SnazzCraft::World* IWorld, int32_t IChunkX, int32_t IChunkZ);

    virtual ~GenerationTaskChunk() = default;

    virtual void Execute() const;

private:


};