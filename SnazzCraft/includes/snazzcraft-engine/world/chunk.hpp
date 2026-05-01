#pragma once

#include <unordered_map>
#include <vector>
#include <array>
#include <mutex>

#include "external/glm/glm.hpp"

#include "snazzcraft-engine/world/voxel.hpp"
#include "snazzcraft-engine/utilities/math.hpp"
#include "snazzcraft-engine/mesh/mesh.hpp"

namespace SnazzCraft
{
    class World;
    class HeightMap;
    class Hitbox;

    constexpr int8_t VoxelCheckPositions[6][3] = {
        {  0,  0, -1 }, // Front
        { -1,  0,  0 }, // Left
        {  1,  0,  0 }, // Right
        {  0,  0,  1 }, // Back
        {  0,  1,  0 }, // Top
        {  0, -1,  0 }  // Bottom
    };

    class Chunk
    {
    public:
        int32_t Position[2]; // X & Z Chunk Coordinates

        SnazzCraft::Mesh* ChunkMesh;

        std::unordered_map<uint32_t, SnazzCraft::Voxel> Voxels; // Voxel positioning is in local chunk space
        std::unordered_map<uint32_t, int> LightValues;
        Chunk(int32_t X, int32_t Y); // Chunk Coordinates 

        ~Chunk();

        void Generate(SnazzCraft::HeightMap* HeightMap, uint32_t MapWidth);

        void UpdateVerticesAndIndices();

        void CullVoxelFaces(); // Clears previously optimized voxels and repopulates the std::unordered_map

        bool VoxelTouchingChunkBorder(uint32_t VoxelIndex, uint32_t* BorderDirection) const;

        SnazzCraft::Voxel* GetCollidingVoxel(const glm::vec3& Position, SnazzCraft::Hitbox* Hitbox); // Returns nullptr if no collision

        SnazzCraft::Voxel* GetCollidingVoxel(const glm::vec3& Position);

        SnazzCraft::Voxel* GetCollidingVoxel(const glm::vec3& Position, SnazzCraft::Hitbox* Hitbox, int32_t LocalVoxelX, int32_t LocalVoxelY, int32_t LocalVoxelZ);

        void UpdateLightingOnVertices(SnazzCraft::World* World);

        inline void UpdateMesh();

    private:
        glm::vec3 ChunkWorldOffset;

        std::vector<SnazzCraft::VoxelVertice> Vertices;
        std::vector<uint32_t> Indices;

        SnazzCraft::Hitbox* VoxelCollisionHitbox;

        inline glm::vec3 LocalVoxelPositionToWorldPosition(uint32_t X, uint32_t Y, uint32_t Z) const;

        inline void WorldSpaceToVoxelSpace(const glm::vec3& WorldPosition, int32_t VoxelPosition[3]) const;

    public:
        static constexpr int16_t Width  = 16;
        static constexpr int16_t Height = 256;
        static constexpr int16_t Depth  = 16;
        static constexpr int16_t OceanLevel = 20; // 125
        static constexpr int16_t MaxOceanDepth = 40;

        static inline void GetChunkPosition(const glm::vec3& Position, int32_t OutChunkPosition[2]); // Voxel space coordinates

        static inline void GetChunkPosition(int32_t X, int32_t Z, int32_t OutChunkPosition[2]); // Voxel space coordinates

        static inline void GetLocalVoxelPosition(int32_t X, int32_t Y, int32_t Z, int32_t OutLocalVoxelPosition[3]); // Voxel space coordinates

        static constexpr uint32_t LocalVoxelIndex(int32_t X, int32_t Y, int32_t Z);

        static constexpr uint32_t LocalVoxelIndex(uint32_t X, uint32_t Y, uint32_t Z);

        static inline uint32_t LocalVoxelIndex(const SnazzCraft::Voxel& Voxel);

        static constexpr bool WithinChunkBounds(int32_t X, int32_t Y, int32_t Z);

        static constexpr bool WithinChunkBounds(uint32_t X, uint32_t Y, uint32_t Z);

    };
} // SnazzCraft

#include "snazzcraft-engine/world/chunk.inl"



