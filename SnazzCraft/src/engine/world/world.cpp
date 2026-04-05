#include "snazzcraft-engine/world/world.hpp"

SnazzCraft::World* SnazzCraft::CurrentWorld = nullptr;

SnazzCraft::World::World(std::string IName, uint32_t ISize, int32_t ISeed)
    : Name(IName), Size(ISize), Seed(ISeed), Chunks(std::unordered_map<uint32_t, SnazzCraft::Chunk*>())
{
    this->WorldHeightMap = new SnazzCraft::HeightMap(this->Size * SnazzCraft::Chunk::Width, -SnazzCraft::Chunk::MaxOceanDepth, SnazzCraft::Chunk::Height - SnazzCraft::Chunk::OceanLevel, this->Seed, 1.0, 0.5, 2.0, 6);
}

SnazzCraft::World::~World()
{
    for (auto& ChunkPair : this->Chunks) {
        delete ChunkPair.second;
    }

    delete this->WorldHeightMap;
}

void SnazzCraft::World::RenderChunks(SnazzCraft::User* Player)
{ 
    int32_t PlayerChunkPosition[2];
    SnazzCraft::Chunk::GetChunkPosition(Player->Position, PlayerChunkPosition);

    for (int32_t X = PlayerChunkPosition[0] - static_cast<int32_t>(this->RenderDistance); X <= PlayerChunkPosition[0] + static_cast<int32_t>(this->RenderDistance); X++) {
    for (int32_t Z = PlayerChunkPosition[1] - static_cast<int32_t>(this->RenderDistance); Z <= PlayerChunkPosition[1] + static_cast<int32_t>(this->RenderDistance); Z++) {
        if (X < 0 || X >= static_cast<int32_t>(this->Size) || Z < 0 || Z >= static_cast<int32_t>(this->Size)) continue;

        auto ChunkIterator = this->Chunks.find(INDEX_2D(X, Z, static_cast<int32_t>(this->Size)));
        if (ChunkIterator == this->Chunks.end()) continue;

        if (ChunkIterator->second->ChunkMesh == nullptr) continue; 
        ChunkIterator->second->ChunkMesh->Draw();
    }
    }
}









