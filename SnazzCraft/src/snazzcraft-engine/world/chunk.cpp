#include "snazzcraft-engine/world/chunk.hpp"
#include "snazzcraft-engine/world/world.hpp"
#include "snazzcraft-engine/utilities/math.hpp"
#include "snazzcraft-engine/texture/atlas.hpp"
#include "snazzcraft-engine/hitbox/hitbox.hpp"
#include "snazzcraft-engine/world/voxel-ids.h"
#include "snazzcraft-engine/height-map/height-map.hpp"
#include "snazzcraft-engine/hitbox/hitbox.hpp"
#include "snazzcraft-engine/utilities/math.hpp"
#include "snazzcraft-engine/world/voxel-type.hpp"

SnazzCraft::Chunk::Chunk(int32_t X, int32_t Y)
: ChunkMesh(nullptr),                               Voxels(std::unordered_map<uint32_t, SnazzCraft::Voxel>()), 
  LightValues(std::unordered_map<uint32_t, int>()), Vertices(std::vector<SnazzCraft::VoxelVertice>()), 
  Indices(std::vector<uint32_t>()),                 VoxelCollisionHitbox(new SnazzCraft::Hitbox(glm::vec3((float)SnazzCraft::Voxel::Size)))
{
    this->Position[0] = X;
    this->Position[1] = Y;

    this->ChunkWorldOffset = {
        static_cast<float>(this->Position[0] * SnazzCraft::Chunk::Width * static_cast<int32_t>(SnazzCraft::Voxel::Size)),
        0.0f,
        static_cast<float>(this->Position[1] * SnazzCraft::Chunk::Depth * static_cast<int32_t>(SnazzCraft::Voxel::Size))
    };
}

SnazzCraft::Chunk::~Chunk()
{
    delete this->ChunkMesh;
    delete this->VoxelCollisionHitbox;
}

void SnazzCraft::Chunk::Generate(SnazzCraft::HeightMap* HeightMap, uint32_t MapWidth)
{
    int32_t HeightMapOffsetX = this->Position[0] * SnazzCraft::Chunk::Width;
    int32_t HeightMapOffsetZ = this->Position[1] * SnazzCraft::Chunk::Depth;

    for (int16_t X = 0; X < SnazzCraft::Chunk::Width; X++) {
    for (int16_t Z = 0; Z < SnazzCraft::Chunk::Depth; Z++) {
        HeightMap->GenerateValue(X + HeightMapOffsetX, Z + HeightMapOffsetZ);

        auto HeightAtPositionIterator = HeightMap->HeightValues.find(SnazzCraft::IntegerHash(X + HeightMapOffsetX, Z + HeightMapOffsetZ));
        if (HeightAtPositionIterator == HeightMap->HeightValues.end()) continue;

        int32_t HeightToGenerate = HeightAtPositionIterator->second + SnazzCraft::Chunk::OceanLevel;
        int32_t Y = 0;
        while (true)
        {
            uint8_t NewVoxelID = ID_VOXEL_STONE;

            if (HeightToGenerate != 0 && Y == HeightToGenerate - 1) {
                if (Y >= SnazzCraft::Chunk::OceanLevel - 1 && Y <= SnazzCraft::Chunk::OceanLevel + 2) {
                    NewVoxelID = ID_VOXEL_SAND;
                } else if (Y >= SnazzCraft::Chunk::OceanLevel - 1) {
                    NewVoxelID = ID_VOXEL_DIRT_GRASS_MIX;
                } else {
                    NewVoxelID = ID_VOXEL_DIRT; 
                }
            } else if (HeightToGenerate != 0 && Y >= HeightToGenerate - 4) {
                NewVoxelID = ID_VOXEL_DIRT;
            }

            SnazzCraft::Voxel NewVoxel(X, Y, Z, NewVoxelID);
            this->Voxels.insert({ SnazzCraft::Chunk::LocalVoxelIndex(X, Y, Z), NewVoxel });

            Y++;
            if (Y >= HeightToGenerate) {
                if (Y >= SnazzCraft::Chunk::OceanLevel) break;
                
                while (Y < SnazzCraft::Chunk::OceanLevel)
                {
                    SnazzCraft::Voxel NewVoxel(X, Y, Z, ID_VOXEL_WATER);
                    this->Voxels.insert({ SnazzCraft::Chunk::LocalVoxelIndex(X, Y, Z), NewVoxel });
                    Y++;
                }
                
                break;
            }
        }

        // Testing Torches
        if (X != 5 || Z != 5 || HeightToGenerate <= SnazzCraft::Chunk::OceanLevel) continue;
        
        SnazzCraft::Voxel NewVoxel = SnazzCraft::Voxel(X, HeightToGenerate, Z, ID_VOXEL_TORCH);
        this->Voxels.insert({ SnazzCraft::Chunk::LocalVoxelIndex(X, HeightToGenerate, Z), NewVoxel });
    }
    }
}

void SnazzCraft::Chunk::UpdateVerticesAndIndices()
{
    this->Vertices.clear();
    this->Indices.clear();

    for (const auto& VoxelPair : this->Voxels) {
        if (VoxelPair.second.GetSideCount() == 0) continue;

        const glm::vec3 Offset = glm::vec3((float)VoxelPair.second.X, (float)VoxelPair.second.Y, (float)VoxelPair.second.Z) * glm::vec3((float)SnazzCraft::Voxel::Size, (float)SnazzCraft::Voxel::Size, (float)SnazzCraft::Voxel::Size) + this->ChunkWorldOffset; 
        uint32_t NewVerticesCount = 0;

        for (SnazzCraft::VoxelVertice& VoxelVertice : SnazzCraft::EngineVoxelTextureApplier->GetTexturedVertices(VoxelPair.second)) { 
            VoxelVertice.Position += Offset; // Adjusting to world space once now means not having to create a new model matrix for each individual chunk later

            this->Vertices.push_back(VoxelVertice);
            NewVerticesCount++; 
        }
        
        for (uint32_t SideIndex = 0; SideIndex < 6; SideIndex++) {
            if (!VoxelPair.second.HasSide(SideIndex)) continue;

            for (uint32_t I = 0; I < 6; I++) {
                this->Indices.push_back(SnazzCraft::VoxelMesh->Indices[SnazzCraft::Index2D<uint32_t>(I, SideIndex, 6)] + this->Vertices.size() - NewVerticesCount);
            }
            
        }
    }
}

void SnazzCraft::Chunk::CullVoxelFaces()
{
    for (auto& VoxelPair : this->Voxels)  {
        VoxelPair.second.SetAllSides();
        const SnazzCraft::VoxelType& PairType = VoxelPair.second.GetVoxelType();

        for (int8_t I = 5; I >= 0; I--) {
            if (!SnazzCraft::AccessBitValue(PairType.CullableSides, I)) continue;

            int32_t CheckPosition[3] = {
                (int)(VoxelPair.second.X) + SnazzCraft::VoxelCheckPositions[I][0],
                (int)(VoxelPair.second.Y) + SnazzCraft::VoxelCheckPositions[I][1],
                (int)(VoxelPair.second.Z) + SnazzCraft::VoxelCheckPositions[I][2]
            };

            if (!SnazzCraft::Chunk::WithinChunkBounds(CheckPosition[0], CheckPosition[1], CheckPosition[2])) continue;

            auto CurrentIterator = this->Voxels.find(SnazzCraft::Chunk::LocalVoxelIndex(CheckPosition[0], CheckPosition[1], CheckPosition[2]));
            if (CurrentIterator == this->Voxels.end()) continue;

            if (!SnazzCraft::AccessBitValue(CurrentIterator->second.GetVoxelType().CullableSides, I)) continue;
      
            VoxelPair.second.ChangeSideValue(I, false);
        }
    }
}

bool SnazzCraft::Chunk::VoxelTouchingChunkBorder(uint32_t VoxelIndex, uint32_t* BorderDirection) const
{
    auto VoxelIterator = this->Voxels.find(VoxelIndex);
    if (VoxelIterator == this->Voxels.end()) return false;

    for (uint32_t I = 0; I < 6; I++) {
        int32_t CheckX = static_cast<int>(VoxelIterator->second.X) + SnazzCraft::VoxelCheckPositions[I][0];
        int32_t CheckY = static_cast<int>(VoxelIterator->second.Y) + SnazzCraft::VoxelCheckPositions[I][1];
        int32_t CheckZ = static_cast<int>(VoxelIterator->second.Z) + SnazzCraft::VoxelCheckPositions[I][2];

        if (!SnazzCraft::Chunk::WithinChunkBounds(CheckX, CheckY, CheckZ)) {
            if (BorderDirection != nullptr) *BorderDirection = I;

            return true;
        }
    }

    return false;
}

SnazzCraft::Voxel* SnazzCraft::Chunk::GetCollidingVoxel(const glm::vec3& Position, SnazzCraft::Hitbox* Hitbox, bool TestEntityCollidablility, bool TestVoxelCollidablility)
{
    int32_t Range[3] = {
        static_cast<int>(glm::ceil(Hitbox->HalfDimensions[0])),
        static_cast<int>(glm::ceil(Hitbox->HalfDimensions[1])),
        static_cast<int>(glm::ceil(Hitbox->HalfDimensions[2]))
    };

    int32_t VPosition[3];
    this->WorldSpaceToVoxelSpace(Position, VPosition);

    for (int32_t X = VPosition[0] - Range[0]; X <= VPosition[0] + Range[0]; X++) {
    for (int32_t Y = VPosition[1] - Range[1]; Y <= VPosition[1] + Range[1]; Y++) {
    for (int32_t Z = VPosition[2] - Range[2]; Z <= VPosition[2] + Range[2]; Z++) {
        SnazzCraft::Voxel* CollidingVoxel = this->GetCollidingVoxel(Position, Hitbox, X, Y, Z, TestEntityCollidablility, TestVoxelCollidablility);
        if (CollidingVoxel == nullptr) continue;

        if (TestEntityCollidablility && CollidingVoxel->GetVoxelType().CollidableToEntities) return CollidingVoxel;
        if (TestVoxelCollidablility  && CollidingVoxel->GetVoxelType().CollidableToVoxels)   return CollidingVoxel;
    }
    }
    }

    return nullptr;
}

SnazzCraft::Voxel* SnazzCraft::Chunk::GetCollidingVoxel(const glm::vec3& Position, bool TestEntityCollidablility, bool TestVoxelCollidablility)
{
    int32_t VPosition[3];
    glm::vec3 CheckPosition = Position;
    this->WorldSpaceToVoxelSpace(CheckPosition, VPosition);

    if (!SnazzCraft::Chunk::WithinChunkBounds(VPosition[0], VPosition[1], VPosition[2])) return nullptr;

    auto VoxelIterator = this->Voxels.find(SnazzCraft::Chunk::LocalVoxelIndex(VPosition[0], VPosition[1], VPosition[2]));
    if (VoxelIterator == this->Voxels.end()) return nullptr;

    if (!this->VoxelCollisionHitbox->IsColliding(this->LocalVoxelPositionToWorldPosition(VPosition[0], VPosition[1], VPosition[2]), CheckPosition)) return nullptr;

    if (TestEntityCollidablility && VoxelIterator->second.GetVoxelType().CollidableToEntities) return &VoxelIterator->second;
    if (TestVoxelCollidablility  && VoxelIterator->second.GetVoxelType().CollidableToVoxels)   return &VoxelIterator->second;

    return nullptr;
}

SnazzCraft::Voxel* SnazzCraft::Chunk::GetCollidingVoxel(const glm::vec3& Position, SnazzCraft::Hitbox* Hitbox, int32_t LocalVoxelX, int32_t LocalVoxelY, int32_t LocalVoxelZ, bool TestEntityCollidablility, bool TestVoxelCollidablility)
{
    if (!SnazzCraft::Chunk::WithinChunkBounds(LocalVoxelX, LocalVoxelY, LocalVoxelZ)) return nullptr;

    auto VoxelIterator = this->Voxels.find(SnazzCraft::Chunk::LocalVoxelIndex(LocalVoxelX, LocalVoxelY, LocalVoxelZ));
    if (VoxelIterator == this->Voxels.end()) return nullptr;

    if (!this->VoxelCollisionHitbox->IsColliding(this->LocalVoxelPositionToWorldPosition(LocalVoxelX, LocalVoxelY, LocalVoxelZ), *Hitbox, Position)) return nullptr;

    if (TestEntityCollidablility && VoxelIterator->second.GetVoxelType().CollidableToEntities) return &VoxelIterator->second;
    if (TestVoxelCollidablility  && VoxelIterator->second.GetVoxelType().CollidableToVoxels)   return &VoxelIterator->second;

    return nullptr;
}

void SnazzCraft::Chunk::UpdateLightingOnVertices(SnazzCraft::World* World)
{
    auto GetLightValue = [this, World](const SnazzCraft::Voxel& Voxel, int8_t Side) -> float
    {
        constexpr float DefaultLightValue = 1.0f / 20.0f;

        int32_t CheckX = static_cast<int32_t>(Voxel.X) + SnazzCraft::VoxelCheckPositions[Side][0];
        int32_t CheckY = static_cast<int32_t>(Voxel.Y) + SnazzCraft::VoxelCheckPositions[Side][1];
        int32_t CheckZ = static_cast<int32_t>(Voxel.Z) + SnazzCraft::VoxelCheckPositions[Side][2];

        if (CheckY >= SnazzCraft::Chunk::Height) return static_cast<float>(SnazzCraft::Voxel::SunlightLightValue) / SnazzCraft::Voxel::MaxLightValue;
        if (CheckY < 0) return DefaultLightValue;

        int32_t TargetChunkX = this->Position[0];
        int32_t TargetChunkZ = this->Position[1];

        if (CheckX < 0) { 
            CheckX += SnazzCraft::Chunk::Width; 
            TargetChunkX--; 
        } else if (CheckX >= SnazzCraft::Chunk::Width) { 
            CheckX -= SnazzCraft::Chunk::Width; 
            TargetChunkX++; 
        }

        if (CheckZ < 0) { 
            CheckZ += SnazzCraft::Chunk::Depth; 
            TargetChunkZ--; 
        } else if (CheckZ >= SnazzCraft::Chunk::Depth) { 
            CheckZ -= SnazzCraft::Chunk::Depth; 
            TargetChunkZ++; 
        }

        if (!World->ChunkWithinWorld(TargetChunkX, TargetChunkZ)) return DefaultLightValue;

        uint64_t ChunkIndex = SnazzCraft::IntegerHash<int32_t>(TargetChunkX, TargetChunkZ);
        auto ChunkIterator = World->Chunks.find(ChunkIndex);
        if (ChunkIterator == World->Chunks.end()) {
            World->GenerateChunk(TargetChunkX, TargetChunkZ, true);
            ChunkIterator = World->Chunks.find(ChunkIndex);
        }
        if (ChunkIterator == World->Chunks.end()) return DefaultLightValue;

        uint32_t LocalIndex = SnazzCraft::Chunk::LocalVoxelIndex(CheckX, CheckY, CheckZ);
        auto LightValueIterator = ChunkIterator->second->LightValues.find(LocalIndex);
        if (LightValueIterator == ChunkIterator->second->LightValues.end() || LightValueIterator->second <= 1) return DefaultLightValue;

        return static_cast<float>(LightValueIterator->second) / static_cast<float>(SnazzCraft::Voxel::MaxLightValue);
    };

    uint32_t VoxelCount = 0;
    for (const auto& [Key, Voxel] : this->Voxels) {
        if (Voxel.GetSideCount() == 0) continue;

        for (uint8_t L = 0x00; L < 0x06; L++) { // 6 faces per voxel
            if (!Voxel.HasSide(L)) continue;

            float LightValue = GetLightValue(Voxel, L);
            for (uint8_t J = 0x00; J < 0x04; J++) { // 4 vertices per face
                this->Vertices[(VoxelCount * 24) + SnazzCraft::Index2D<uint8_t>(J, L, 4)].Brightness = LightValue; // 24 vertices per voxel
            }
        }

        VoxelCount++;
    }
}

