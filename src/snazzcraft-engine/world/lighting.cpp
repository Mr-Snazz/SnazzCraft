#include "snazzcraft-engine/world/world.hpp"
#include "snazzcraft-engine/chunk/chunk.hpp"
#include "snazzcraft-engine/utilities/math.hpp"
#include "snazzcraft-engine/voxel/voxel-type.hpp"
#include "snazzcraft-engine/world/light-node.hpp"

void SnazzCraft::World::UpdateChunkLighting(SnazzCraft::Chunk* Chunk, bool* UpdatedInputChunk)
{
    std::vector<SnazzCraft::Chunk*> RegionChunks;
    RegionChunks.reserve(9);
    for (int32_t DX = -1; DX <= 1; ++DX) {
    for (int32_t DZ = -1; DZ <= 1; ++DZ) {
        int32_t CX = Chunk->Position[0] + DX;
        int32_t CZ = Chunk->Position[1] + DZ;
        if (!this->ChunkWithinWorld(CX, CZ)) continue;

        uint64_t Hash = SnazzCraft::IntegerHash<int32_t>(CX, CZ);
        auto ChunkIterator = this->Chunks.find(Hash);
        if (ChunkIterator == this->Chunks.end()) continue;

        RegionChunks.push_back(ChunkIterator->second);
    }
    }

    for (SnazzCraft::Chunk* Chunk : RegionChunks) Chunk->ClearLightValues();

    std::unordered_set<uint64_t> ChunksToUpdate;

    for (SnazzCraft::Chunk* Chunk : RegionChunks) {
        this->ApplySunLightingToChunk(Chunk, &ChunksToUpdate);

        for (uint32_t VoxelIndex{}; VoxelIndex < SnazzCraft::Chunk::Volume; VoxelIndex++) {
            const SnazzCraft::Voxel& Voxel = Chunk->Voxels[VoxelIndex];

            int32_t LightProducingLevel = Voxel.GetVoxelType().LightProducingLevel;
            if (LightProducingLevel <= 0) continue;

            int32_t VoxelX, VoxelY, VoxelZ;
            SnazzCraft::Chunk::GetVoxelPosition(VoxelIndex, VoxelX, VoxelY, VoxelZ);
            int32_t Position[3] = {
                VoxelX + Chunk->Position[0] * SnazzCraft::Chunk::Width,
                VoxelY,
                VoxelZ + Chunk->Position[1] * SnazzCraft::Chunk::Depth,
            };
            this->ApplyLightingVoxel(Position, LightProducingLevel, &ChunksToUpdate);
        }
    }

    for (uint64_t I : ChunksToUpdate) {
        auto ChunkIterator = this->Chunks.find(I);
        if (ChunkIterator == this->Chunks.end()) continue;

        ChunkIterator->second->UpdateLightingOnVertices(this);
        ChunkIterator->second->ShouldUpdateMesh = true;

        SnazzCraft::Chunk* UpdatedChunk = ChunkIterator->second;
        for (int32_t DX = -1; DX <= 1; ++DX) {
        for (int32_t DZ = -1; DZ <= 1; ++DZ) {
            if (!DX && !DZ) continue;

            int32_t NeighborChunkX = UpdatedChunk->Position[0] + DX;
            int32_t NeighborChunkZ = UpdatedChunk->Position[1] + DZ;
            if (!this->ChunkWithinWorld(NeighborChunkX, NeighborChunkZ)) continue;

            uint64_t NeighborHash = SnazzCraft::IntegerHash<int32_t>(NeighborChunkX, NeighborChunkZ);
            auto NeighborIterator = this->Chunks.find(NeighborHash);
            if (NeighborIterator == this->Chunks.end()) continue;

            NeighborIterator->second->UpdateLightingOnVertices(this);
            NeighborIterator->second->ShouldUpdateMesh = true;
        }
        }
    }

    auto ChunkIterator = ChunksToUpdate.find(SnazzCraft::IntegerHash<int32_t>(Chunk->Position[0], Chunk->Position[1]));
    if (UpdatedInputChunk) *UpdatedInputChunk = ChunkIterator != ChunksToUpdate.end();
}

void SnazzCraft::World::ApplySunLightingToChunk(SnazzCraft::Chunk* Chunk, std::unordered_set<uint64_t>* ChunksToUpdate)
{
    for (uint32_t Z = 0; Z < SnazzCraft::Chunk::Depth; Z++) {
    for (uint32_t X = 0; X < SnazzCraft::Chunk::Width; X++) {
        this->ApplySunLightingToColumn(Chunk, X, Z, SnazzCraft::Chunk::Height, SnazzCraft::Voxel::SunlightLightValue, ChunksToUpdate);
    }
    }

    if (ChunksToUpdate) ChunksToUpdate->insert(SnazzCraft::IntegerHash<int32_t>(Chunk->Position[0], Chunk->Position[1]));
}

void SnazzCraft::World::ApplySunLightingToColumn(SnazzCraft::Chunk* Chunk, uint32_t LocalChunkX, uint32_t LocalChunkZ, uint32_t StartY, int8_t StartLightValue, std::unordered_set<uint64_t>* ChunksToUpdate)
{
    if (!Chunk) return;

    int LightValue = SnazzCraft::Voxel::SunlightLightValue;
    for (uint32_t Y = StartY; Y > 0; Y--) {
        if (Y >= SnazzCraft::Chunk::Height) continue;
        uint32_t LightY = Y - 1;
        uint32_t LocalIndex = SnazzCraft::Chunk::LocalVoxelIndex(LocalChunkX, LightY, LocalChunkZ);

        int ChunkLightValue = Chunk->LightValues[LocalIndex];
        Chunk->LightValues[LocalIndex] = LightValue > ChunkLightValue ? LightValue : ChunkLightValue;

        const SnazzCraft::Voxel& Voxel = Chunk->Voxels[LocalIndex];
        
        LightValue -= Voxel.GetVoxelType().LightPropogationDecrease;
        if (LightValue > 1) continue;

        int32_t LightOrigin[3] = {
            static_cast<int32_t>(LocalChunkX) + Chunk->Position[0] * SnazzCraft::Chunk::Width,
            static_cast<int32_t>(LightY + 1),
            static_cast<int32_t>(LocalChunkZ) + Chunk->Position[1] * SnazzCraft::Chunk::Depth
        };
        this->ApplyLightingVoxel(LightOrigin, LightValue, ChunksToUpdate);

        break;
    }

    ChunksToUpdate->insert(SnazzCraft::IntegerHash(Chunk->Position[0], Chunk->Position[1]));
}

void SnazzCraft::World::ApplyLightingVoxel(int32_t LightOrigin[3], int8_t LightProducingLevel, std::unordered_set<uint64_t>* ChunksToUpdate)
{
    auto VoxelIsOutsideWorld = [this](int32_t X, int32_t Y, int32_t Z) -> bool
    {
        return X <  -this->Size      * SnazzCraft::Chunk::Width || Y <  0                         || Z <  -this->Size      * SnazzCraft::Chunk::Depth || 
               X >= (this->Size + 1) * SnazzCraft::Chunk::Width || Y >= SnazzCraft::Chunk::Height || Z >= (this->Size + 1) * SnazzCraft::Chunk::Depth;
    };

    auto AddLightNodes = [](std::queue<SnazzCraft::World::LightNode>& Queue, const SnazzCraft::World::LightNode& OriginNode, int LightPropagationDecrease) -> void
    {
        int32_t NewLightValue = OriginNode.LightValue - LightPropagationDecrease;
        if (NewLightValue <= 0) return;

        for (uint8_t I{}; I < 0x03; ++I) {
            int32_t NewPosition[3] = {
                OriginNode.X,
                OriginNode.Y,
                OriginNode.Z
            };

            // Add in both directons on axis specified by I
            NewPosition[I]--;
            Queue.push(SnazzCraft::World::LightNode(NewLightValue, NewPosition));

            NewPosition[I] += 2;
            Queue.push(SnazzCraft::World::LightNode(NewLightValue, NewPosition));
        }
    };

    std::queue<SnazzCraft::World::LightNode> Queue;
    SnazzCraft::World::LightNode LightOriginNode(LightProducingLevel, LightOrigin);
    Queue.push(LightOriginNode);

    while (!Queue.empty())
    {
        SnazzCraft::World::LightNode CurrentNode = std::move(Queue.front());
        Queue.pop();

        if (VoxelIsOutsideWorld(CurrentNode.X, CurrentNode.Y, CurrentNode.Z)) continue;

        int32_t ChunkCoordinates[2];
        SnazzCraft::Chunk::GetChunkPosition(CurrentNode.X, CurrentNode.Z, ChunkCoordinates);
        uint64_t ChunkHash = SnazzCraft::IntegerHash(ChunkCoordinates[0], ChunkCoordinates[1]);

        auto ChunkIterator = this->Chunks.find(ChunkHash);
        if (ChunkIterator == this->Chunks.end()) continue;

        int32_t Local[3];
        SnazzCraft::Chunk::GetLocalVoxelPosition(CurrentNode.X, CurrentNode.Y, CurrentNode.Z, Local);
        int32_t LightIndex = SnazzCraft::Chunk::LocalVoxelIndex(Local[0], Local[1], Local[2]);

        int8_t ExistingLightValue = ChunkIterator->second->LightValues[LightIndex];

        if (CurrentNode.LightValue > ExistingLightValue) {
            ChunkIterator->second->LightValues[LightIndex] = CurrentNode.LightValue;

            if (ChunksToUpdate) ChunksToUpdate->insert(SnazzCraft::IntegerHash<int32_t>(ChunkCoordinates[0], ChunkCoordinates[1]));
            
            int32_t LocalVoxelPosition[3];
            SnazzCraft::Chunk::GetLocalVoxelPosition(CurrentNode.X, CurrentNode.Y, CurrentNode.Z, LocalVoxelPosition);

            const SnazzCraft::Voxel& Voxel = ChunkIterator->second->Voxels[SnazzCraft::Chunk::LocalVoxelIndex(LocalVoxelPosition[0], LocalVoxelPosition[1], LocalVoxelPosition[2])];
            int8_t VoxelLightPropogationDecrease =  Voxel.GetVoxelType().LightPropogationDecrease;
            AddLightNodes(Queue, CurrentNode, (VoxelLightPropogationDecrease > 1) ? VoxelLightPropogationDecrease : 1);
        }
    }
}