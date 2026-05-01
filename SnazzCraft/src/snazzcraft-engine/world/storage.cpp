#include "snazzcraft-engine/world/world.hpp"
#include "snazzcraft-engine/world/chunk.hpp"
#include "snazzcraft-engine/utilities/math.hpp"
#include "snazzcraft-engine/utilities/text-file-handling.hpp"

bool SnazzCraft::World::SaveWorldToFile(bool OverwriteExistingFile) const
{
    constexpr const char* FileDirectory = "worlds/";
    constexpr const char* FileExtension = ".txt";

    std::string FilePath = FileDirectory + this->Name + FileExtension;

    if (std::filesystem::exists(FilePath) && !OverwriteExistingFile) return false;

    std::ofstream File(FilePath);

    File << WORLD_SAVE_FILE_DESCRIPTOR_NAME       << ": " << this->Name << "\n";
    File << WORLD_SAVE_FILE_DESCRIPTOR_SIZE       << ": " << this->Size << "\n";
    File << WORLD_SAVE_FILE_DESCRIPTOR_WORLD_SEED << ": " << this->Seed << "\n";

    for (auto& ChunkPair : this->Chunks) {
        SnazzCraft::Chunk* Chunk = ChunkPair.second;

        File << WORLD_SAVE_FILE_DESCRIPTOR_CHUNK_BEGIN << ": " << Chunk->Position[0] << " " << Chunk->Position[1] << "\n";

        for (auto& VoxelPair : Chunk->Voxels) {
            File << WORLD_SAVE_FILE_DESCRIPTOR_CHUNK_NEW_VOXEL << ": " << VoxelPair.second.X << " " << VoxelPair.second.Y << " " << VoxelPair.second.Z << " " << VoxelPair.second.ID << "\n";
        }

        File << WORLD_SAVE_FILE_DESCRIPTOR_CHUNK_END << ":\n";
    }

    File.close();
    return true;
}

SnazzCraft::World* SnazzCraft::World::LoadWorldFromSaveFile(std::string FilePath)
{
    std::ifstream File(FilePath);
    if (!File.is_open()) return nullptr;

    std::string NewWorldName;
    uint32_t NewWorldSize;
    int32_t NewWorldSeed;

    std::string Line;

    std::vector<std::pair<int32_t, int32_t>> ChunkPositionsToAdd;
    std::vector<std::vector<SnazzCraft::Voxel>> VoxelsToAddToChunks;

    char EmptyChar = ' ';
    while (std::getline(File, Line))
    {
        if (Line.size() == 0) continue;
        if (Line[0] == ' ')   continue;

        uint32_t LineIndex = 3;
        std::string Data;

        SnazzCraft::ParseData(Data, Line, LineIndex, NULL);
       
        switch (Line[0])
        {
            case WORLD_SAVE_FILE_DESCRIPTOR_NAME:
            {
                uint32_t DataIndex = 0;
                SnazzCraft::ParseData(NewWorldName, Data, DataIndex, NULL);
                break;
            }

            case WORLD_SAVE_FILE_DESCRIPTOR_SIZE:
            {
                std::string Size;
                uint32_t DataIndex = 0;
                SnazzCraft::ParseData(Size, Data, DataIndex, &EmptyChar);

                NewWorldSize = static_cast<uint32_t >(stoul(Size));
                break;
            }

            case WORLD_SAVE_FILE_DESCRIPTOR_WORLD_SEED:
            {
                std::string Seed;
                uint32_t DataIndex = 0;
                SnazzCraft::ParseData(Seed, Data, DataIndex, &EmptyChar);

                NewWorldSeed = stoi(Seed);
                break;
            }

            case WORLD_SAVE_FILE_DESCRIPTOR_CHUNK_BEGIN:
            {
                std::string NewCoordinate;
                int32_t NewCoordinates_Int[2];
                uint32_t DataIndex = 0;

                SnazzCraft::ParseData(NewCoordinate, Data, DataIndex, &EmptyChar);

                NewCoordinates_Int[0] = stoi(NewCoordinate);
                NewCoordinate.clear();

                DataIndex++;
                SnazzCraft::ParseData(NewCoordinate, Data, DataIndex, &EmptyChar);
                NewCoordinates_Int[1] = stoi(NewCoordinate);

                ChunkPositionsToAdd.push_back({ NewCoordinates_Int[0], NewCoordinates_Int[1] });

                VoxelsToAddToChunks.emplace_back();
                break;
            }
                

            case WORLD_SAVE_FILE_DESCRIPTOR_CHUNK_END:
            {
                break;
            }    

            case WORLD_SAVE_FILE_DESCRIPTOR_CHUNK_NEW_VOXEL:
            {
                uint32_t DataIndex = 0;
                std::string NewInfo;
                uint32_t NewVoxelInfo[4]; // X, Y, Z, ID

                for (uint32_t I = 0; I < 3; I++) {
                    SnazzCraft::ParseData(NewInfo, Data, DataIndex, &EmptyChar);
                    NewVoxelInfo[I] = static_cast<uint32_t >(stoul(NewInfo));
                    NewInfo.clear();
                    DataIndex++;
                }

                SnazzCraft::ParseData(NewInfo, Data, DataIndex, &EmptyChar); 
                NewVoxelInfo[3] = static_cast<uint32_t >(stoul(NewInfo));

                SnazzCraft::Voxel NewVoxel(NewVoxelInfo[0], NewVoxelInfo[1], NewVoxelInfo[2], NewVoxelInfo[3]);\
                VoxelsToAddToChunks.back().push_back(NewVoxel);

                break;
            }
        }
    }
    
    SnazzCraft::World* NewWorld = new SnazzCraft::World(NewWorldName, NewWorldSize, NewWorldSeed);

    for (size_t I = 0; I < ChunkPositionsToAdd.size(); I++) {
        std::pair<int32_t, int32_t> NewChunkPosition = ChunkPositionsToAdd[I];
        SnazzCraft::Chunk* NewChunk = new SnazzCraft::Chunk(NewChunkPosition.first, NewChunkPosition.second);

        for (SnazzCraft::Voxel Voxel : VoxelsToAddToChunks[I]) {
            NewChunk->Voxels.insert_or_assign(SnazzCraft::Chunk::LocalVoxelIndex(Voxel), Voxel);
        }

        NewChunk->CullVoxelFaces();
        NewChunk->UpdateVerticesAndIndices();

        NewChunk->UpdateMesh();
        NewWorld->Chunks.insert_or_assign(SnazzCraft::Index2D(NewChunkPosition.first, NewChunkPosition.second, NewWorld->Size), NewChunk);
    }
    
    for (auto& [Key, Chunk] : NewWorld->Chunks) {
        NewWorld->UpdateChunkLighting(Chunk, nullptr);
    }

    File.close();

    return NewWorld;
}