#include "snazzcraft-engine/world/world.hpp"
#include "snazzcraft-engine/world/chunk.hpp"
#include "snazzcraft-engine/utilities/math.hpp"
#include "snazzcraft-engine/height-map/height-map.hpp"

void SnazzCraft::World::GenerateChunk(int32_t X, int32_t Z, bool ApplyLighting)
{
    if (X >= SnazzCraft::World::Size || Z >= SnazzCraft::World::Size) return;

    uint64_t ChunkHash = SnazzCraft::IntegerHash(X, Z);
    auto Iterator = this->Chunks.find(ChunkHash);
    if (Iterator != this->Chunks.end()) return;
    
    SnazzCraft::Chunk* NewChunk = new SnazzCraft::Chunk(X, Z);

    NewChunk->Generate(this->WorldHeightMap, static_cast<uint32_t>(this->Size * SnazzCraft::Chunk::Width));
    NewChunk->CullVoxelFaces();
    NewChunk->UpdateVerticesAndIndices();   

    this->Chunks[ChunkHash] = NewChunk;
    bool UpdatedChunk;
    if (ApplyLighting) { 
        this->UpdateChunkLighting(NewChunk, &UpdatedChunk); 
    }

    if (!UpdatedChunk) {
        NewChunk->UpdateLightingOnVertices(this);
        NewChunk->UpdateMesh();
    }
}

SnazzCraft::World* SnazzCraft::World::CreateWorld(std::string Name, uint32_t Size, int32_t Seed)
{
    uint32_t GenerateSize = Size == 0 || Size > SnazzCraft::World::MaxSize ? 
        SnazzCraft::World::MaxSize : 
        Size;

    SnazzCraft::World* NewWorld = new SnazzCraft::World(Name, GenerateSize, Seed);

    for (int32_t X = -NewWorld->Size; X <= NewWorld->Size; X++) {
    for (int32_t Z = -NewWorld->Size; Z <= NewWorld->Size; Z++) {
        NewWorld->GenerateChunk(X, Z, true);
    } 
    }
    
    return NewWorld;
}

void SnazzCraft::World::UpdateChunkVerticeLightingAndMesh(uint64_t Index)
{
    auto ChunkIterator = this->Chunks.find(Index);
    if (ChunkIterator == this->Chunks.end()) return;

    this->UpdateChunkVerticeLightingAndMesh(ChunkIterator->second);
}

void SnazzCraft::World::UpdateChunkVerticeLightingAndMesh(SnazzCraft::Chunk* Chunk)
{
    Chunk->UpdateLightingOnVertices(this);
    Chunk->UpdateMesh();
}
