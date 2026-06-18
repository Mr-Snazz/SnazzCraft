#include "snazzcraft-engine/world/world.hpp"
#include "snazzcraft-engine/chunk/chunk.hpp"
#include "snazzcraft-engine/utilities/math.hpp"
#include "snazzcraft-engine/height-map/height-map.hpp"

void SnazzCraft::World::GenerateChunk(int32_t X, int32_t Z)
{
    if (!this->ChunkWithinWorld(X, Z)) return;

    uint64_t ChunkHash = SnazzCraft::IntegerHash(X, Z);

    {
        std::lock_guard<std::mutex> ChunksLock(this->ChunksMutex);
    
        auto Iterator = this->Chunks.find(ChunkHash);
        if (Iterator != this->Chunks.end()) return;
    }
    
    SnazzCraft::Chunk* NewChunk = new SnazzCraft::Chunk(X, Z);

    {
        std::lock_guard<std::mutex> HeightMapLock(this->HeightMapMutex);
        NewChunk->Generate(this->HeightMap, static_cast<uint32_t>(this->Size * SnazzCraft::Chunk::Width));
    }
    
    NewChunk->CullVoxelFaces();
    NewChunk->UpdateVerticesAndIndices();   

    {
        std::lock_guard<std::mutex> ChunksLock(this->ChunksMutex);

        this->Chunks[ChunkHash] = NewChunk;

        bool UpdatedChunk = false;
        this->UpdateChunkLighting(NewChunk, &UpdatedChunk); 

        if (UpdatedChunk) return;
            
        NewChunk->UpdateLightingOnVertices(this);
        NewChunk->ShouldUpdateMesh = true;
    }
}

SnazzCraft::World* SnazzCraft::World::CreateWorld(std::string Name, uint32_t Size, int32_t Seed)
{
    uint32_t GenerateSize = Size == 0u || Size > SnazzCraft::World::MaxSize ? 
                            SnazzCraft::World::MaxSize : Size;

    SnazzCraft::World* NewWorld = new SnazzCraft::World(Name, GenerateSize, Seed);
    
    for (int32_t X = -NewWorld->Size; X <= NewWorld->Size; X++) {
    for (int32_t Z = -NewWorld->Size; Z <= NewWorld->Size; Z++) {
        NewWorld->ThreadPool.Enqueue([NewWorld, X, Z](){ NewWorld->GenerateChunk(X, Z); });
    } 
    }

    return NewWorld;
}

