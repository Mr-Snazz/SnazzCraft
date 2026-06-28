#include "snazzcraft-engine/chunk/chunk.hpp"
#include "snazzcraft-engine/world/world.hpp"
#include "snazzcraft-engine/utilities/math.hpp"
#include "snazzcraft-engine/texture/atlas.hpp"
#include "snazzcraft-engine/hitbox/hitbox.hpp"
#include "snazzcraft-engine/height-map/height-map.hpp"
#include "snazzcraft-engine/hitbox/hitbox.hpp"
#include "snazzcraft-engine/utilities/math.hpp"
#include "snazzcraft-engine/voxel/voxel-type.hpp"
#include "snazzcraft-engine/mesh/mesh.hpp"

SnazzCraft::Chunk::Chunk(int32_t X, int32_t Y)
    : Voxels(std::array<SnazzCraft::Voxel, SnazzCraft::Chunk::Volume>()), 
    LightValues(std::array<int8_t, SnazzCraft::Chunk::Volume>()), 
    ShouldUpdateMesh(false),
    ChunkMesh(SnazzCraft::ChunkMesh({}, {}, false)), 
    VoxelCollisionHitbox(new SnazzCraft::Hitbox(glm::vec3((float)SnazzCraft::Voxel::Size)))
{
    this->Position[0] = X;
    this->Position[1] = Y;

    this->WorldOffset = {
        static_cast<float>(this->Position[0] * SnazzCraft::Chunk::Width * static_cast<int32_t>(SnazzCraft::Voxel::Size)),
        0.0f,
        static_cast<float>(this->Position[1] * SnazzCraft::Chunk::Depth * static_cast<int32_t>(SnazzCraft::Voxel::Size))
    };

    for (uint16_t X{}; X < SnazzCraft::Chunk::Width;  X++) {
    for (uint16_t Y{}; Y < SnazzCraft::Chunk::Height; Y++) {
    for (uint16_t Z{}; Z < SnazzCraft::Chunk::Depth;  Z++) {
        uint32_t LocalIndex = SnazzCraft::Chunk::LocalVoxelIndex(X, Y, Z);

        this->Voxels     [LocalIndex] = SnazzCraft::Voxel(SnazzCraft::VoxelType::VoxelTypeID::Air);
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

    for (int16_t X{}; X < SnazzCraft::Chunk::Width; X++) {
    for (int16_t Z{}; Z < SnazzCraft::Chunk::Depth; Z++) {
        HeightMap->GenerateValue(X + HeightMapOffsetX, Z + HeightMapOffsetZ);

        auto HeightAtPositionIterator = HeightMap->HeightValues.find(SnazzCraft::IntegerHash(X + HeightMapOffsetX, Z + HeightMapOffsetZ));
        if (HeightAtPositionIterator == HeightMap->HeightValues.end()) continue;

        int32_t HeightToGenerate = HeightAtPositionIterator->second + SnazzCraft::Chunk::OceanLevel;
        int32_t Y{};
        while (true)
        {
            SnazzCraft::VoxelType::VoxelTypeID NewVoxelID = SnazzCraft::VoxelType::VoxelTypeID::Stone;

            if (HeightToGenerate && Y == HeightToGenerate - 1) {
                if (Y >= SnazzCraft::Chunk::OceanLevel - 1 && Y <= SnazzCraft::Chunk::OceanLevel + 2) {
                    NewVoxelID = SnazzCraft::VoxelType::VoxelTypeID::Sand;
                } else if (Y >= SnazzCraft::Chunk::OceanLevel - 1) {
                    NewVoxelID = SnazzCraft::VoxelType::VoxelTypeID::DirtGrassMix;
                } else {
                    NewVoxelID = SnazzCraft::VoxelType::VoxelTypeID::Dirt;
                }
            } else if (HeightToGenerate != 0 && Y >= HeightToGenerate - 4) {
                NewVoxelID = SnazzCraft::VoxelType::VoxelTypeID::Dirt;
            }

            SnazzCraft::Voxel NewVoxel(NewVoxelID);
            this->Voxels[SnazzCraft::Chunk::LocalVoxelIndex(X, Y, Z)] = NewVoxel;

            Y++;
            if (Y >= HeightToGenerate) {
                if (Y >= SnazzCraft::Chunk::OceanLevel) break;
                
                while (Y < SnazzCraft::Chunk::OceanLevel)
                {
                    SnazzCraft::Voxel NewVoxel(SnazzCraft::VoxelType::VoxelTypeID::Water);
                    this->Voxels[SnazzCraft::Chunk::LocalVoxelIndex(X, Y, Z)] = NewVoxel;
                    Y++;
                }
                
                break;
            }
        }

        // Testing Torches
        if (X != 5 || Z != 5 || HeightToGenerate <= SnazzCraft::Chunk::OceanLevel) continue;
        
        SnazzCraft::Voxel NewVoxel = SnazzCraft::Voxel(SnazzCraft::VoxelType::VoxelTypeID::Torch);
        this->Voxels[SnazzCraft::Chunk::LocalVoxelIndex(X, HeightToGenerate, Z)] = NewVoxel;
    }
    }
}

void SnazzCraft::Chunk::UpdateVerticesAndIndices()
{
    this->ChunkMesh.Vertices.clear();
    this->ChunkMesh.Indices.clear();

    for (uint32_t VoxelIndex{}; VoxelIndex < SnazzCraft::Chunk::Volume; VoxelIndex++) {
        const SnazzCraft::Voxel& Voxel = this->Voxels[VoxelIndex];

        if (!Voxel.GetSideCount()) continue;

        uint8_t VoxelX, VoxelY, VoxelZ;
        SnazzCraft::Chunk::GetVoxelPosition(VoxelIndex, VoxelX, VoxelY, VoxelZ);

        const glm::vec3 Offset = glm::vec3(VoxelX * SnazzCraft::Voxel::Size, VoxelY * SnazzCraft::Voxel::Size, VoxelZ * SnazzCraft::Voxel::Size);

        uint32_t NewVerticesCount{};
        for (SnazzCraft::VoxelVertice& VoxelVertice : SnazzCraft::EngineVoxelTextureApplier->GetTexturedVertices(Voxel)) { 
            VoxelVertice.Position += Offset; // Adjusting to world space once now means not having to create a new model matrix for each individual chunk later

            this->ChunkMesh.Vertices.push_back(VoxelVertice);
            NewVerticesCount++; 
        }
        
        for (uint32_t SideIndex{}; SideIndex < 6; SideIndex++) {
            if (!Voxel.HasSide(SideIndex)) continue;

            for (uint32_t I{}; I < 6u; I++) {
                this->ChunkMesh.Indices.push_back(SnazzCraft::VoxelMesh->Indices[SnazzCraft::Index2D<uint32_t>(I, SideIndex, 6)] + this->ChunkMesh.Vertices.size() - NewVerticesCount);
            }
            
        }
    }

    this->ShouldUpdateMesh = true;
}

void SnazzCraft::Chunk::CullVoxelFaces()
{
    for (uint32_t VoxelIndex{}; VoxelIndex < SnazzCraft::Chunk::Volume; VoxelIndex++) {
        SnazzCraft::Voxel& Voxel = this->Voxels[VoxelIndex];

        if (Voxel.ID == SnazzCraft::VoxelType::VoxelTypeID::Air) {
            Voxel.ClearAllSides();
            continue;
        }

        Voxel.SetAllSides();
        const SnazzCraft::VoxelType& VoxelType = Voxel.GetVoxelType();

        int32_t VoxelX, VoxelY, VoxelZ;
        SnazzCraft::Chunk::GetVoxelPosition(VoxelIndex, VoxelX, VoxelY, VoxelZ);

        for (int8_t I = 5u; I >= 0; I--) {
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

    for (uint32_t I{}; I < 6u; ++I) {
        int32_t CheckX = VoxelX + SnazzCraft::VoxelCheckPositions[I][0];
        int32_t CheckY = VoxelY + SnazzCraft::VoxelCheckPositions[I][1];
        int32_t CheckZ = VoxelZ + SnazzCraft::VoxelCheckPositions[I][2];

        if (!SnazzCraft::Chunk::WithinChunkBounds(CheckX, CheckY, CheckZ)) {
            if (BorderDirection) *BorderDirection = I;

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
    for (int32_t X = VPosition[0] - Range[0]; X <= VPosition[0] + Range[0]; ++X) {
    for (int32_t Y = VPosition[1] - Range[1]; Y <= VPosition[1] + Range[1]; ++Y) {
    for (int32_t Z = VPosition[2] - Range[2]; Z <= VPosition[2] + Range[2]; ++Z) {
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
    uint32_t VoxelCount{};
    for (uint32_t VoxelIndex{}; VoxelIndex < SnazzCraft::Chunk::Volume; VoxelIndex++) {
        const SnazzCraft::Voxel& Voxel = this->Voxels[VoxelIndex];

        if (!Voxel.GetSideCount()) continue;
        // Compute per-corner brightness (8 corners) by sampling the surrounding voxels
        constexpr float DefaultLightValueFloat = static_cast<float>(SnazzCraft::Voxel::DefaultLightValue) / static_cast<float>(SnazzCraft::Voxel::MaxLightValue);

        int32_t VoxelX, VoxelY, VoxelZ;
        SnazzCraft::Chunk::GetVoxelPosition(VoxelIndex, VoxelX, VoxelY, VoxelZ);

        float CornerBrightness[8];
        for (uint8_t Corner{}; Corner < 8u; ++Corner) {
            int8_t SX = (Corner & 1) ? 1 : -1;
            int8_t SY = (Corner & 2) ? 1 : -1;
            int8_t SZ = (Corner & 4) ? 1 : -1;

            int32_t XS[2] = {
                (SX == 1) ? VoxelX : VoxelX - 1,
                (SX == 1) ? VoxelX + 1 : VoxelX
            };
            int32_t YS[2] = {
                (SY == 1) ? VoxelY : VoxelY - 1,
                (SY == 1) ? VoxelY + 1 : VoxelY
            };
            int32_t ZS[2] = {
                (SZ == 1) ? VoxelZ : VoxelZ - 1,
                (SZ == 1) ? VoxelZ + 1 : VoxelZ
            };

            uint32_t Total{};
            uint32_t SampleCount{};
            for (uint8_t XI{}; XI < 2u; ++XI) {
            for (uint8_t YI{}; YI < 2u; ++YI) {
            for (uint8_t ZI{}; ZI < 2u; ++ZI) {
                int32_t X = XS[XI];
                int32_t Y = YS[YI];
                int32_t Z = ZS[ZI];

                if (Y >= SnazzCraft::Chunk::Height) { Total += SnazzCraft::Voxel::SunlightLightValue; SampleCount++; continue; }
                if (Y < 0) continue;

                int32_t TargetChunkX = this->Position[0];
                int32_t TargetChunkZ = this->Position[1];

                int32_t LocalX = X;
                int32_t LocalZ = Z;

                if (LocalX < 0) { LocalX += SnazzCraft::Chunk::Width; TargetChunkX--; }
                else if (LocalX >= SnazzCraft::Chunk::Width) { LocalX -= SnazzCraft::Chunk::Width; TargetChunkX++; }

                if (LocalZ < 0) { LocalZ += SnazzCraft::Chunk::Depth; TargetChunkZ--; }
                else if (LocalZ >= SnazzCraft::Chunk::Depth) { LocalZ -= SnazzCraft::Chunk::Depth; TargetChunkZ++; }

                if (!World->ChunkWithinWorld(TargetChunkX, TargetChunkZ)) { Total += SnazzCraft::Voxel::DefaultLightValue; SampleCount++; continue; }

                uint64_t ChunkIndex = SnazzCraft::IntegerHash<int32_t>(TargetChunkX, TargetChunkZ);
                auto ChunkIterator = World->Chunks.find(ChunkIndex);
                if (ChunkIterator == World->Chunks.end()) { Total += SnazzCraft::Voxel::DefaultLightValue; SampleCount++; continue; }

                int8_t LightValue = ChunkIterator->second->LightValues[SnazzCraft::Chunk::LocalVoxelIndex(LocalX, Y, LocalZ)];
                if (LightValue <= 1) { Total += SnazzCraft::Voxel::DefaultLightValue; SampleCount++; continue; }

                Total += LightValue;
                SampleCount++;
            }
            }
            }

            if (SampleCount == 0) CornerBrightness[Corner] = DefaultLightValueFloat;
            else {
                float RawAverage = static_cast<float>(Total) / static_cast<float>(SampleCount);
                CornerBrightness[Corner] = RawAverage / static_cast<float>(SnazzCraft::Voxel::MaxLightValue);
            }
        }

        // Assign per-vertex brightness based on corner mapping
        for (uint8_t L{}; L < 6u; ++L) {
            if (!Voxel.HasSide(L)) continue;

            for (uint8_t J{}; J < 4u; ++J) {
                const SnazzCraft::Vertice& Vertice = SnazzCraft::VoxelMesh->Vertices[SnazzCraft::Index2D<uint8_t>(J, L, 4u)];
                uint8_t CornderIndex = (Vertice.Position[0] > 0.0f ? 1u : 0u) | (Vertice.Position[1] > 0.0f ? 2u : 0u) | (Vertice.Position[2] > 0.0f ? 4u : 0u);
                this->ChunkMesh.Vertices[(VoxelCount * 24u) + SnazzCraft::Index2D<uint8_t>(J, L, 4u)].Brightness = CornerBrightness[CornderIndex];
            }
        }
        VoxelCount++;
    }

    this->ShouldUpdateMesh = true;
}

float SnazzCraft::Chunk::GetLightValue(SnazzCraft::World* World, uint32_t VoxelIndex, uint8_t Side, uint8_t VerticeCount)
{
    constexpr float DefaultLightValueFloat = static_cast<float>(SnazzCraft::Voxel::DefaultLightValue) / static_cast<float>(SnazzCraft::Voxel::MaxLightValue);

    int32_t VoxelX, VoxelY, VoxelZ;
    SnazzCraft::Chunk::GetVoxelPosition(VoxelIndex, VoxelX, VoxelY, VoxelZ);

    // First voxel is always one step facing the normal of the face
    int32_t BaseX = VoxelX + SnazzCraft::VoxelCheckPositions[Side][0];
    int32_t BaseY = VoxelY + SnazzCraft::VoxelCheckPositions[Side][1];
    int32_t BaseZ = VoxelZ + SnazzCraft::VoxelCheckPositions[Side][2];

    // Find out whether we should increase or decrease on each axis
    int32_t Mod[3] = { 0, 0, 0 };
    const SnazzCraft::Vertice& Vertice = SnazzCraft::VoxelMesh->Vertices[VerticeCount % 4u];
    Mod[0] = (Vertice.Position[0] < 0.0f) ? -1 : 1;
    Mod[1] = (Vertice.Position[1] < 0.0f) ? -1 : 1;
    Mod[2] = (Vertice.Position[2] < 0.0f) ? -1 : 1;

    // The axis in which the side's normal is facing (both negative and positive) will not change
    switch (Side) 
    {
        case 0: // Front -Z
        case 3: // Back +Z
            Mod[2] = 0;
            break;

        case 1: // Left -X
        case 2: // Right +X
            Mod[0] = 0;
            break;

        case 4: // Top +Y
        case 5: // Bottom -Y
            Mod[1] = 0;
            break;

        default: // Invalid side
            return DefaultLightValueFloat;
    }

    int32_t Total{};
    uint32_t SampleCount{};
    for (int IX = 0; IX <= (Mod[0] == 0 ? 0 : 1); ++IX) {
    for (int IY = 0; IY <= (Mod[1] == 0 ? 0 : 1); ++IY) {
    for (int IZ = 0; IZ <= (Mod[2] == 0 ? 0 : 1); ++IZ) {
        int32_t X = BaseX + (IX ? Mod[0] : 0);
        int32_t Y = BaseY + (IY ? Mod[1] : 0);
        int32_t Z = BaseZ + (IZ ? Mod[2] : 0);

        if (Y >= SnazzCraft::Chunk::Height) { Total += SnazzCraft::Voxel::SunlightLightValue; SampleCount++; continue; }
        if (Y < 0) continue;

        int32_t TargetChunkX = this->Position[0];
        int32_t TargetChunkZ = this->Position[1];

        int32_t LocalX = X;
        int32_t LocalZ = Z;

        if (LocalX < 0) { 
            LocalX += SnazzCraft::Chunk::Width; 
            TargetChunkX--; 
        } else if (LocalX >= SnazzCraft::Chunk::Width) { 
            LocalX -= SnazzCraft::Chunk::Width; 
            TargetChunkX++; 
        }

        if (LocalZ < 0) { 
            LocalZ += SnazzCraft::Chunk::Depth; 
            TargetChunkZ--; 
        } else if (LocalZ >= SnazzCraft::Chunk::Depth) {
            LocalZ -= SnazzCraft::Chunk::Depth; 
            TargetChunkZ++; 
        }

        if (!World->ChunkWithinWorld(TargetChunkX, TargetChunkZ)) { Total += SnazzCraft::Voxel::DefaultLightValue; SampleCount++; continue; }

        uint64_t ChunkIndex = SnazzCraft::IntegerHash<int32_t>(TargetChunkX, TargetChunkZ);
        auto ChunkIterator = World->Chunks.find(ChunkIndex);
        if (ChunkIterator == World->Chunks.end()) { Total += SnazzCraft::Voxel::DefaultLightValue; SampleCount++; continue; }

        int8_t LightValue = ChunkIterator->second->LightValues[SnazzCraft::Chunk::LocalVoxelIndex(LocalX, Y, LocalZ)];
        if (LightValue <= 1) { Total += SnazzCraft::Voxel::DefaultLightValue; SampleCount++; continue; }

        Total += LightValue;
        SampleCount++;
    }
    }
    }

    if (SampleCount == 0u) return DefaultLightValueFloat;

    float AverageRaw = static_cast<float>(Total) / static_cast<float>(SampleCount);
    return AverageRaw / static_cast<float>(SnazzCraft::Voxel::MaxLightValue);
}