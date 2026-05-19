#include "snazzcraft-engine/world/world.hpp"
#include "snazzcraft-engine/chunk/chunk.hpp"
#include "snazzcraft-engine/utilities/math.hpp"
#include "snazzcraft-engine/height-map/height-map.hpp"

void SnazzCraft::World::GenerateChunk(int32_t X, int32_t Z, bool ApplyLighting)
{
    if (!this->ChunkWithinWorld(X, Z)) return;

    std::lock_guard<std::mutex> ChunksLock(this->ChunksMutex);
    std::lock_guard<std::mutex> HeightMapLock(this->HeighhtMapMutex);

    uint64_t ChunkHash = SnazzCraft::IntegerHash(X, Z);
    auto Iterator = this->Chunks.find(ChunkHash);
    if (Iterator != this->Chunks.end()) return;
    
    SnazzCraft::Chunk* NewChunk = new SnazzCraft::Chunk(X, Z);

    NewChunk->Generate(this->HeightMap, static_cast<uint32_t>(this->Size * SnazzCraft::Chunk::Width));
    NewChunk->CullVoxelFaces();
    NewChunk->UpdateVerticesAndIndices();   

    this->Chunks[ChunkHash] = NewChunk;
    bool UpdatedChunk = false;
    if (ApplyLighting) this->UpdateChunkLighting(NewChunk, &UpdatedChunk); 

    //if (UpdatedChunk) return;
        
    NewChunk->UpdateLightingOnVertices(this);
    NewChunk->ShouldUpdateMesh = true;
}

SnazzCraft::World* SnazzCraft::World::CreateWorld(std::string Name, uint32_t Size, int32_t Seed)
{
    uint32_t GenerateSize = Size == 0u || Size > SnazzCraft::World::MaxSize ? 
        SnazzCraft::World::MaxSize : 
        Size;

    SnazzCraft::World* NewWorld = new SnazzCraft::World(Name, GenerateSize, Seed);
    
    for (int32_t X = -NewWorld->Size; X <= NewWorld->Size; X++) {
    for (int32_t Z = -NewWorld->Size; Z <= NewWorld->Size; Z++) {
        NewWorld->ThreadPool.Enqueue([NewWorld, X, Z](void* Argument){ NewWorld->GenerateChunk(X, Z, true); }, nullptr);
    } 
    }

    //std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    return NewWorld;
}

