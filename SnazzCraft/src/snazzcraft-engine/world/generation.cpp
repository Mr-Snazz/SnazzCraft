#include "snazzcraft-engine/world/world.hpp"
#include "snazzcraft-engine/chunk/chunk.hpp"
#include "snazzcraft-engine/utilities/math.hpp"
#include "snazzcraft-engine/height-map/height-map.hpp"
#include "snazzcraft-engine/world/generation-task/generation-task-chunk.hpp"

void SnazzCraft::World::GenerateChunk(int32_t X, int32_t Z, bool ApplyLighting)
{
    if (!this->ChunkWithinWorld(X, Z)) return;

    //std::lock_guard<std::mutex> ChunksLock(this->ChunksMutex);
    //std::lock_guard<std::mutex> HeightMapLock(this->HeighhtMapMutex);

    uint64_t ChunkHash = SnazzCraft::IntegerHash(X, Z);
    auto Iterator = this->Chunks.find(ChunkHash);
    if (Iterator != this->Chunks.end()) return;
    
    SnazzCraft::Chunk* NewChunk = new SnazzCraft::Chunk(X, Z);

    NewChunk->Generate(this->HeightMap, static_cast<uint32_t>(this->Size * SnazzCraft::Chunk::Width));
    NewChunk->CullVoxelFaces(this);
    NewChunk->UpdateVerticesAndIndices();   

    this->Chunks[ChunkHash] = NewChunk;
    bool UpdatedChunk = false;
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
    uint32_t GenerateSize = Size == 0u || Size > SnazzCraft::World::MaxSize ? 
        SnazzCraft::World::MaxSize : 
        Size;

    SnazzCraft::World* NewWorld = new SnazzCraft::World(Name, GenerateSize, Seed);

    //std::vector<SnazzCraft::World::GenerationTask*> Tasks;
    for (int32_t X = -NewWorld->Size; X <= NewWorld->Size; X++) {
    for (int32_t Z = -NewWorld->Size; Z <= NewWorld->Size; Z++) {
        //NewWorld->GenerateChunk(X, Z, true);
        //Tasks.push_back(new SnazzCraft::World::GenerationTaskChunk(NewWorld, X, Z));
        //SnazzCraft::World::GenerationTask* New = new SnazzCraft::World::GenerationTaskChunk(NewWorld, X, Z);
        //Test.Enqueue([New](){ New->Execute(); });
    } 
    }

    //for (const auto& Task : Tasks) Task->Execute();


    return NewWorld;
}

