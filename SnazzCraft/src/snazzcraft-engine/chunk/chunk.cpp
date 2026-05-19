#include "snazzcraft-engine/chunk/chunk.hpp"
#include "snazzcraft-engine/world/world.hpp"
#include "snazzcraft-engine/utilities/math.hpp"
#include "snazzcraft-engine/texture/atlas.hpp"
#include "snazzcraft-engine/hitbox/hitbox.hpp"
#include "snazzcraft-engine/voxel/voxel-ids.h"
#include "snazzcraft-engine/height-map/height-map.hpp"
#include "snazzcraft-engine/hitbox/hitbox.hpp"
#include "snazzcraft-engine/utilities/math.hpp"
#include "snazzcraft-engine/voxel/voxel-type.hpp"

SnazzCraft::Chunk::Chunk(int32_t X, int32_t Y)
    : Voxels(std::array<SnazzCraft::Voxel, SnazzCraft::Chunk::Volume>()), 
    LightValues(std::array<int32_t, SnazzCraft::Chunk::Volume>()), 
    ShouldUpdateMesh(false),
    ShouldUpdateLighting(false),
    ChunkMesh(SnazzCraft::Mesh({}, {}, false)), 
    VoxelCollisionHitbox(new SnazzCraft::Hitbox(glm::vec3((float)SnazzCraft::Voxel::Size)))
{
    this->Position[0] = X;
    this->Position[1] = Y;

    this->ChunkWorldOffset = {
        static_cast<float>(this->Position[0] * SnazzCraft::Chunk::Width * static_cast<int32_t>(SnazzCraft::Voxel::Size)),
        0.0f,
        static_cast<float>(this->Position[1] * SnazzCraft::Chunk::Depth * static_cast<int32_t>(SnazzCraft::Voxel::Size))
    };

    for (uint16_t X = 0u; X < SnazzCraft::Chunk::Width;  X++) {
    for (uint16_t Y = 0u; Y < SnazzCraft::Chunk::Height; Y++) {
    for (uint16_t Z = 0u; Z < SnazzCraft::Chunk::Depth;  Z++) {
        uint32_t LocalIndex = SnazzCraft::Chunk::LocalVoxelIndex(X, Y, Z);

        this->Voxels     [LocalIndex] = SnazzCraft::Voxel(ID_VOXEL_AIR);
        this->LightValues[LocalIndex] = SnazzCraft::Voxel::DefaultLightValue;
    }
    }
    }
}

SnazzCraft::Chunk::~Chunk()
{
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

            SnazzCraft::Voxel NewVoxel(NewVoxelID);
            this->Voxels[SnazzCraft::Chunk::LocalVoxelIndex(X, Y, Z)] = NewVoxel;

            Y++;
            if (Y >= HeightToGenerate) {
                if (Y >= SnazzCraft::Chunk::OceanLevel) break;
                
                while (Y < SnazzCraft::Chunk::OceanLevel)
                {
                    SnazzCraft::Voxel NewVoxel(ID_VOXEL_WATER);
                    this->Voxels[SnazzCraft::Chunk::LocalVoxelIndex(X, Y, Z)] = NewVoxel;
                    Y++;
                }
                
                break;
            }
        }

        // Testing Torches
        if (X != 5 || Z != 5 || HeightToGenerate <= SnazzCraft::Chunk::OceanLevel) continue;
        
        SnazzCraft::Voxel NewVoxel = SnazzCraft::Voxel(ID_VOXEL_TORCH);
        this->Voxels[SnazzCraft::Chunk::LocalVoxelIndex(X, HeightToGenerate, Z)] = NewVoxel;
    }
    }
}

void SnazzCraft::Chunk::UpdateVerticesAndIndices()
{
    this->ChunkMesh.Vertices.clear();
    this->ChunkMesh.Indices.clear();

    for (uint32_t VoxelIndex = 0u; VoxelIndex < SnazzCraft::Chunk::Volume; VoxelIndex++) {
        const SnazzCraft::Voxel& Voxel = this->Voxels[VoxelIndex];

        if (Voxel.GetSideCount() == 0u) continue;

        uint8_t VoxelX, VoxelY, VoxelZ;
        SnazzCraft::Chunk::GetVoxelPosition(VoxelIndex, VoxelX, VoxelY, VoxelZ);

        const glm::vec3 Offset = glm::vec3((float)VoxelX, (float)VoxelY, (float)VoxelZ) * glm::vec3((float)SnazzCraft::Voxel::Size, (float)SnazzCraft::Voxel::Size, (float)SnazzCraft::Voxel::Size) + this->ChunkWorldOffset; 

        uint32_t NewVerticesCount = 0;
        for (SnazzCraft::VoxelVertice& VoxelVertice : SnazzCraft::EngineVoxelTextureApplier->GetTexturedVertices(Voxel)) { 
            VoxelVertice.Position += Offset; // Adjusting to world space once now means not having to create a new model matrix for each individual chunk later

            this->ChunkMesh.Vertices.push_back(VoxelVertice);
            NewVerticesCount++; 
        }
        
        for (uint32_t SideIndex = 0; SideIndex < 6; SideIndex++) {
            if (!Voxel.HasSide(SideIndex)) continue;

            for (uint32_t I = 0; I < 6; I++) {
                this->ChunkMesh.Indices.push_back(SnazzCraft::VoxelMesh->Indices[SnazzCraft::Index2D<uint32_t>(I, SideIndex, 6)] + this->ChunkMesh.Vertices.size() - NewVerticesCount);
            }
            
        }
    }

    this->ShouldUpdateMesh = true;
}

void SnazzCraft::Chunk::CullVoxelFaces()
{
    for (uint32_t VoxelIndex = 0u; VoxelIndex < SnazzCraft::Chunk::Volume; VoxelIndex++) {
        SnazzCraft::Voxel& Voxel = this->Voxels[VoxelIndex];

        if (Voxel.ID == ID_VOXEL_AIR) {
            Voxel.ClearAllSides();
            continue;
        }

        Voxel.SetAllSides();
        const SnazzCraft::VoxelType& VoxelType = Voxel.GetVoxelType();

        int32_t VoxelX, VoxelY, VoxelZ;
        SnazzCraft::Chunk::GetVoxelPosition(VoxelIndex, VoxelX, VoxelY, VoxelZ);

        for (int8_t I = 5; I >= 0; I--) {
            if (!SnazzCraft::AccessBitValue(VoxelType.CullableSides, I)) continue;

            int32_t CheckPosition[3] = {
                VoxelX + SnazzCraft::VoxelCheckPositions[I][0],
                VoxelY + SnazzCraft::VoxelCheckPositions[I][1],
                VoxelZ + SnazzCraft::VoxelCheckPositions[I][2]
            };

            if (!SnazzCraft::Chunk::WithinChunkBounds(CheckPosition[0], CheckPosition[1], CheckPosition[2])) continue;

            const SnazzCraft::Voxel& CullAgainstVoxel = this->Voxels[SnazzCraft::Chunk::LocalVoxelIndex(CheckPosition[0], CheckPosition[1], CheckPosition[2])];
            const SnazzCraft::VoxelType& CullAgainstVoxelType = CullAgainstVoxel.GetVoxelType();
            if (!CullAgainstVoxelType.CullableAgainst) continue;

            if (!SnazzCraft::AccessBitValue(CullAgainstVoxelType.CullableSides, I)) continue;
      
            Voxel.ChangeSideValue(I, false);
        }
    }

    this->ShouldUpdateMesh = true;
}

bool SnazzCraft::Chunk::VoxelTouchingChunkBorder(uint32_t VoxelIndex, uint32_t* BorderDirection) const
{
    int32_t VoxelX, VoxelY, VoxelZ;
    SnazzCraft::Chunk::GetVoxelPosition(VoxelIndex, VoxelX, VoxelY, VoxelZ);

    for (uint32_t I = 0; I < 6; I++) {
        int32_t CheckX = VoxelX + SnazzCraft::VoxelCheckPositions[I][0];
        int32_t CheckY = VoxelY + SnazzCraft::VoxelCheckPositions[I][1];
        int32_t CheckZ = VoxelZ + SnazzCraft::VoxelCheckPositions[I][2];

        if (!SnazzCraft::Chunk::WithinChunkBounds(CheckX, CheckY, CheckZ)) {
            if (BorderDirection != nullptr) *BorderDirection = I;

            return true;
        }
    }

    return false;
}

SnazzCraft::World::VoxelCollisionInfo SnazzCraft::Chunk::GetCollidingVoxel(const glm::vec3& Position, SnazzCraft::Hitbox* Hitbox, bool TestEntityCollidablility, bool TestVoxelCollidablility)
{
    int32_t Range[3] = {
        static_cast<int>(glm::ceil(Hitbox->HalfDimensions[0])),
        static_cast<int>(glm::ceil(Hitbox->HalfDimensions[1])),
        static_cast<int>(glm::ceil(Hitbox->HalfDimensions[2]))
    };

    int32_t VPosition[3];
    this->WorldSpaceToVoxelSpace(Position, VPosition);

    SnazzCraft::World::VoxelCollisionInfo VoxelCollisionInfo;
    for (int32_t X = VPosition[0] - Range[0]; X <= VPosition[0] + Range[0]; X++) {
    for (int32_t Y = VPosition[1] - Range[1]; Y <= VPosition[1] + Range[1]; Y++) {
    for (int32_t Z = VPosition[2] - Range[2]; Z <= VPosition[2] + Range[2]; Z++) {
        SnazzCraft::World::VoxelCollisionInfo VoxelCollisinoInfo = this->GetCollidingVoxel(Position, Hitbox, X, Y, Z, TestEntityCollidablility, TestVoxelCollidablility);
        if (VoxelCollisinoInfo.Collided()) return VoxelCollisinoInfo;
    }
    }
    }

    return SnazzCraft::World::VoxelCollisionInfo();
}

SnazzCraft::World::VoxelCollisionInfo SnazzCraft::Chunk::GetCollidingVoxel(const glm::vec3& Position, bool TestEntityCollidablility, bool TestVoxelCollidablility)
{
    int32_t VPosition[3];
    glm::vec3 CheckPosition = Position;
    this->WorldSpaceToVoxelSpace(CheckPosition, VPosition);

    if (!SnazzCraft::Chunk::WithinChunkBounds(VPosition[0], VPosition[1], VPosition[2])) return SnazzCraft::World::VoxelCollisionInfo();

    if (!this->VoxelCollisionHitbox->IsColliding(this->LocalVoxelPositionToWorldPosition(VPosition[0], VPosition[1], VPosition[2]), CheckPosition)) return SnazzCraft::World::VoxelCollisionInfo();

    uint32_t VoxelIndex = SnazzCraft::Chunk::LocalVoxelIndex(VPosition[0], VPosition[1], VPosition[2]);
    SnazzCraft::Voxel& Voxel = this->Voxels[VoxelIndex];

    if (TestEntityCollidablility && Voxel.GetVoxelType().CollidableToEntities) return SnazzCraft::World::VoxelCollisionInfo(&Voxel, this, VoxelIndex);
    if (TestVoxelCollidablility  && Voxel.GetVoxelType().CollidableToVoxels)   return SnazzCraft::World::VoxelCollisionInfo(&Voxel, this, VoxelIndex);

    return SnazzCraft::World::VoxelCollisionInfo();
}

SnazzCraft::World::VoxelCollisionInfo SnazzCraft::Chunk::GetCollidingVoxel(const glm::vec3& Position, SnazzCraft::Hitbox* Hitbox, int32_t LocalVoxelX, int32_t LocalVoxelY, int32_t LocalVoxelZ, bool TestEntityCollidablility, bool TestVoxelCollidablility)
{
    if (!SnazzCraft::Chunk::WithinChunkBounds(LocalVoxelX, LocalVoxelY, LocalVoxelZ)) return SnazzCraft::World::VoxelCollisionInfo();

    if (!this->VoxelCollisionHitbox->IsColliding(this->LocalVoxelPositionToWorldPosition(LocalVoxelX, LocalVoxelY, LocalVoxelZ), *Hitbox, Position)) return SnazzCraft::World::VoxelCollisionInfo();

    uint32_t VoxelIndex = SnazzCraft::Chunk::LocalVoxelIndex(LocalVoxelX, LocalVoxelY, LocalVoxelZ);
    SnazzCraft::Voxel& Voxel = this->Voxels[VoxelIndex];

    if (TestEntityCollidablility && Voxel.GetVoxelType().CollidableToEntities) return SnazzCraft::World::VoxelCollisionInfo(&Voxel, this, VoxelIndex);
    if (TestVoxelCollidablility  && Voxel.GetVoxelType().CollidableToVoxels)   return SnazzCraft::World::VoxelCollisionInfo(&Voxel, this, VoxelIndex);

    return SnazzCraft::World::VoxelCollisionInfo();
}

void SnazzCraft::Chunk::UpdateLightingOnVertices(SnazzCraft::World* World)
{
    auto GetLightValue = [this, World](uint32_t VoxelIndex, int8_t Side) -> float
    {
        constexpr float DefaultLightValueFloat = static_cast<float>(SnazzCraft::Voxel::DefaultLightValue) / static_cast<float>(SnazzCraft::Voxel::MaxLightValue);

        int32_t VoxelX, VoxelY, VoxelZ;
        SnazzCraft::Chunk::GetVoxelPosition(VoxelIndex, VoxelX, VoxelY, VoxelZ);

        int32_t CheckX = VoxelX + SnazzCraft::VoxelCheckPositions[Side][0];
        int32_t CheckY = VoxelY + SnazzCraft::VoxelCheckPositions[Side][1];
        int32_t CheckZ = VoxelZ + SnazzCraft::VoxelCheckPositions[Side][2];

        if (CheckY >= SnazzCraft::Chunk::Height) return static_cast<float>(SnazzCraft::Voxel::SunlightLightValue) / SnazzCraft::Voxel::MaxLightValue;
        if (CheckY < 0) return DefaultLightValueFloat;

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

        if (!World->ChunkWithinWorld(TargetChunkX, TargetChunkZ)) return DefaultLightValueFloat;

        uint64_t ChunkIndex = SnazzCraft::IntegerHash<int32_t>(TargetChunkX, TargetChunkZ);
        auto ChunkIterator = World->Chunks.find(ChunkIndex);
        if (ChunkIterator == World->Chunks.end()) return DefaultLightValueFloat;

        uint32_t LocalIndex = SnazzCraft::Chunk::LocalVoxelIndex(CheckX, CheckY, CheckZ);

        int LightValue = ChunkIterator->second->LightValues[LocalIndex];
        if (LightValue <= 1) return DefaultLightValueFloat;

        return static_cast<float>(LightValue) / static_cast<float>(SnazzCraft::Voxel::MaxLightValue);
    };

    uint32_t VoxelCount = 0u;
    for (uint32_t VoxelIndex = 0u; VoxelIndex < SnazzCraft::Chunk::Volume; VoxelIndex++) {
        const SnazzCraft::Voxel& Voxel = this->Voxels[VoxelIndex];

        if (Voxel.GetSideCount() == 0) continue;

        for (uint8_t L = 0x00; L < 0x06; L++) { // 6 faces per voxel
            if (!Voxel.HasSide(L)) continue;

            float LightValue = GetLightValue(VoxelIndex, L);
            for (uint8_t J = 0x00; J < 0x04; J++) { // 4 vertices per face
                this->ChunkMesh.Vertices[(VoxelCount * 24) + SnazzCraft::Index2D<uint8_t>(J, L, 4)].Brightness = LightValue; // 24 vertices per voxel
            }
        }
        VoxelCount++;
    }

    this->ShouldUpdateMesh = true;
}

