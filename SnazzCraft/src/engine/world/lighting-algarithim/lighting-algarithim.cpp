#include "../world.hpp"

void PropogateSunlight(int X, int Y, int Z, SnazzCraft::World* World, unsigned int LightLevel);

bool OutOfBounds(int X, int Y, int Z, unsigned int WorldSize);

void SnazzCraft::World::UpdateLighting()
{
    for (auto ChunkPair : *this->Chunks) {
    for (auto VoxelPair : *ChunkPair.second->Voxels) {
        if (VoxelPair.second.LightProducingLevel == 0) continue;

        int X = VoxelPair.second.Position[0] + ChunkPair.second->Position[0] * SnazzCraft::Chunk::Width;
        int Y = VoxelPair.second.Position[1];
        int Z = VoxelPair.second.Position[2] + ChunkPair.second->Position[1] * SnazzCraft::Chunk::Depth;

        PropogateSunlight(X, Y, Z, this, VoxelPair.second.LightProducingLevel);
    }
    }
}

void PropogateSunlight(int X, int Y, int Z, SnazzCraft::World* World, unsigned int LightLevel)
{
    auto NewRecursion = [X, Y, Z, World, LightLevel](unsigned int TranslationIndex) -> void
    {
        int NewX = X + SnazzCraft::VoxelCheckPositions[TranslationIndex][0];
        int NewY = Y + SnazzCraft::VoxelCheckPositions[TranslationIndex][1];
        int NewZ = Z + SnazzCraft::VoxelCheckPositions[TranslationIndex][2];

        PropogateSunlight(NewX, NewY, NewZ, World, LightLevel - 1);
    };

    auto GetChunkPosition = [X, Z](int& OutX, int& OutZ) -> void
    {
        OutX = X / SnazzCraft::Chunk::Width;
        OutZ = Z / SnazzCraft::Chunk::Depth;
    };  

    auto GetLocalVoxelPosition = [X, Y, Z](int& OutX, int& OutY, int& OutZ) -> void
    {
        OutX = X % SnazzCraft::Chunk::Width;
        OutY = Y;
        OutZ = Z % SnazzCraft::Chunk::Depth;
    };  

    if (!OutOfBounds(X, Y, Z, World->Size) || LightLevel == 0) return;

    int ChunkX, ChunkZ;
    GetChunkPosition(ChunkX, ChunkZ);
    unsigned int ChunkHash = ChunkZ * World->Size + ChunkX;
    auto ChunkIterator = World->Chunks->find(ChunkHash);
    if (ChunkIterator == World->Chunks->end()) return;

    int VoxelX, VoxelY, VoxelZ;
    GetLocalVoxelPosition(VoxelX, VoxelY, VoxelZ);
    unsigned int VoxelHash = VOXEL_INDEX(VoxelX, VoxelY, VoxelZ);
    auto VoxelIterator = ChunkIterator->second->Voxels->find(VoxelHash);
    if (VoxelIterator == ChunkIterator->second->Voxels->end()) {
        for (unsigned int I = 0; I < 6; I++) {
            NewRecursion(I);
        }

        return;
    }

    for (unsigned int I = 0; I < 6; I++) {
        VoxelIterator->second.FaceLightLevels[I] = VoxelIterator->second.FaceLightLevels[I] >= LightLevel ? VoxelIterator->second.FaceLightLevels[I] : LightLevel;

        NewRecursion(I);
    }

}

bool OutOfBounds(int X, int Y, int Z, unsigned int WorldSize)
{
    return X >= 0 && Y >= 0 && Z >= 0 && X < SnazzCraft::Chunk::Width * WorldSize && Y < SnazzCraft::Chunk::Height && Z < SnazzCraft::Chunk::Depth * WorldSize;
}
