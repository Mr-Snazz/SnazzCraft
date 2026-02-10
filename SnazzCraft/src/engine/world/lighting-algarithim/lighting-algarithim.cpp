#include "../world.hpp"

std::vector<unsigned int> GetRelativeLightingSideIndexes(int LightPosition[3], int VoxelPosition[3]);

void SnazzCraft::World::UpdateLighting()
{
    const int WorldSizeInt = static_cast<int>(this->Size);

    for (auto& ChunkPair : *this->Chunks) {
    for (auto& VoxelPair : *ChunkPair.second->OptimizedVoxels) {
        if (VoxelPair.second.LightProducingLevel == 0) continue;

        int VoxelPositionInt[3] {
            static_cast<int>(VoxelPair.second.Position[0]),
            static_cast<int>(VoxelPair.second.Position[1]),
            static_cast<int>(VoxelPair.second.Position[2])
        };  

        int Range = static_cast<int>(VoxelPair.second.LightProducingLevel);
        for (int X = VoxelPositionInt[0] - Range; X <= VoxelPositionInt[0] + Range; X++) {
        for (int Y = VoxelPositionInt[1] - Range; Y <= VoxelPositionInt[1] + Range; Y++) {
        for (int Z = VoxelPositionInt[2] - Range; Z <= VoxelPositionInt[2] + Range; Z++) {
            if (X < 0 || Y < 0 || Z < 0 || X >= WorldSizeInt || Y >= WorldSizeInt || Z >= WorldSizeInt) continue;

            int CheckPosition[3] = { X, Y, Z };
            unsigned int ChunkIndex = static_cast<unsigned int>(INDEX_2D(X, Z, WorldSizeInt));

            auto ChunkIterator = this->Chunks->find(ChunkIndex);
            if (ChunkIterator == this->Chunks->end()) continue;

            auto VoxelIterator = ChunkIterator->second->Voxels->find(VOXEL_INDEX(CheckPosition[0], CheckPosition[1], CheckPosition[2]));
            if (VoxelIterator == ChunkIterator->second->Voxels->end()) continue;


            

        }
        }
        }
    }
    }
}

std::vector<unsigned int> GetRelativeLightingSideIndexes(int LightPosition[3], int VoxelPosition[3])
{
    std::vector<unsigned int> SideIndexes;

    if (LightPosition[0] > VoxelPosition[0]) {
        SideIndexes.push_back(2); // Right
    } else if (LightPosition[0] < VoxelPosition[0]) {
        SideIndexes.push_back(1); // Left
    }

    if (LightPosition[1] > VoxelPosition[1]) {
        SideIndexes.push_back(4); // Top
    } else if (LightPosition[1] < VoxelPosition[1]) {
        SideIndexes.push_back(5); // Bottom
    }

    if (LightPosition[2] > VoxelPosition[2]) {
        SideIndexes.push_back(3); // Back
    } else if (LightPosition[2] < VoxelPosition[2]) {
        SideIndexes.push_back(0); // Front
    }

    return SideIndexes;
}

