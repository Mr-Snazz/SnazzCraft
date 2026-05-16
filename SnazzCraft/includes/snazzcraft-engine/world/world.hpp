#pragma once

#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
#include <thread>
#include <mutex>
#include <stdint.h>
#include <queue>
#include <functional>

#include "external/glm/glm.hpp"

#include "snazzcraft-engine/entity/entity.hpp"
#include "snazzcraft-engine/voxel/voxel.hpp"
#include "snazzcraft-engine/entity/user.hpp"

#define WORLD_SAVE_FILE_DESCRIPTOR_NAME            ('0')
#define WORLD_SAVE_FILE_DESCRIPTOR_SIZE            ('1')
#define WORLD_SAVE_FILE_DESCRIPTOR_CHUNK_BEGIN     ('2')
#define WORLD_SAVE_FILE_DESCRIPTOR_CHUNK_END       ('3')
#define WORLD_SAVE_FILE_DESCRIPTOR_CHUNK_NEW_VOXEL ('4')
#define WORLD_SAVE_FILE_DESCRIPTOR_WORLD_SEED      ('5')

namespace SnazzCraft
{
    class Chunk;
    class HeightMap;

    class World
    {
    public:
        static constexpr uint32_t MaxSize = 2147483647u;

        std::string Name = "UNASSIGNED";
        int32_t Size;
        int32_t Seed;
        
        uint32_t RenderDistance = 50;
        float PlayerReach = static_cast<float>(SnazzCraft::Voxel::Size * 5);

        std::unordered_map<uint64_t, SnazzCraft::Chunk*> Chunks; // Uses SnazzCraft::IntegerHash for hasing
        std::vector<SnazzCraft::Entity*> Entities;

        World(std::string IName, uint32_t ISize, int32_t ISeed);

        ~World();

        void GenerateChunk(int32_t X, int32_t Z, bool ApplyLighting); 

        void Render() const;

    private:
        class VoxelCollisionInfo
        {
        public:
            SnazzCraft::Voxel* CollidingVoxel;
            SnazzCraft::Chunk* CollidedInChunk;
            uint32_t CollidingVoxelIndex;

            VoxelCollisionInfo();

            VoxelCollisionInfo(SnazzCraft::Voxel* ICollidingVoxel, SnazzCraft::Chunk* ICollidedInChunk, uint32_t ICollidingVoxelIndex);

            inline bool Collided() const;
        };

    public:
        SnazzCraft::World::VoxelCollisionInfo GetCollidingVoxel(const glm::vec3& Position, SnazzCraft::Hitbox* Hitbox, bool TestEntityCollidablility, bool TestVoxelCollidablility) const; // Returns nullptr if no collision 

        SnazzCraft::World::VoxelCollisionInfo GetCollidingVoxel(const glm::vec3& Position, bool TestEntityCollidablility, bool TestVoxelCollidablility) const; // Returns nullptr if no collision - Position should be in world space

        void MoveEntity(SnazzCraft::Entity* Entity, const glm::vec3& Rotation, float Distance) const; // Returns true if movement occurred without voxel collision 

        void MoveEntity(glm::vec3 Translation, SnazzCraft::Entity* Entity) const; // Returns true if movement occurred without voxel collision 

        /*
        Calls UpdateVerticesAndIndices & UpdateMesh on all chunks affected
        If the Chunk in the address given has no light producing voxels then no member functions of that chunk will be called to update its vertices, indices, or mesh
        */
        void UpdateChunkLighting(SnazzCraft::Chunk* Chunk, bool* UpatedInputChunk);

        bool DestroyVoxel(const glm::vec3& Position, const glm::vec3& Rotation);

        bool PlaceVoxel(const glm::vec3& Position, const glm::vec3& Rotation, uint8_t VoxelID);

        void UpdateVoxelPlacementDisplay();

        inline void ApplyGravityToAllEntities() const;

        inline const glm::vec3& GetVoxelPlacementDisplayPosition() const;

        bool ChunkWithinWorld(SnazzCraft::Chunk* Chunk) const;

        inline bool ChunkWithinWorld(int32_t ChunkX, int32_t ChunkZ) const;

    private:
        SnazzCraft::HeightMap* WorldHeightMap = nullptr;
        SnazzCraft::Mesh* VoxelPlacementDisplayMesh = nullptr;
        glm::vec3 VoxelPlacementDisplayPosition;
        bool ShouldRenderVoxelPlacementDisplay = false;

        void RenderAllEntities() const;

        void RenderChunks() const;

        void RenderVoxelPlacementDisplay() const;

        /*
        Rotation is expected to not be normalized
        */
        bool RaycastToVoxel(glm::vec3& Position, const glm::vec3& Rotation, float MaxDistance, uint8_t* FaceHit, SnazzCraft::World::VoxelCollisionInfo* VoxelCollisionInfo); 

        void ApplySunLightingToChunk(SnazzCraft::Chunk* Chunk, std::unordered_set<uint64_t>* ChunksToUpdate);

        void ApplySunLightingToColumn(SnazzCraft::Chunk* Chunk, uint32_t LocalChunkX, uint32_t LocalChunkZ, uint32_t StartY, int32_t StartLightValue, std::unordered_set<uint64_t>* ChunksToUpdate);
        
        /*
        Only to be called trough UpdateChunkLighting
        Generates currently ungenerated Chunks when light values would affect them
        */
        void ApplyLightingVoxel(int32_t LightOrigin[3], int32_t LightProducingLevel, std::unordered_set<uint64_t>* ChunksToUpdate);
        
    public:
        friend class SnazzCraft::Chunk;

        static SnazzCraft::World* CreateWorld(std::string Name, uint32_t Size, int32_t Seed);


    private:
        class LightNode;
        class GenerationTask;
    };
    
    extern SnazzCraft::World* CurrentWorld;
} // SnazzCraft

#include "snazzcraft-engine/world/world.inl"
#include "snazzcraft-engine/world/voxel-collision-info.inl"
