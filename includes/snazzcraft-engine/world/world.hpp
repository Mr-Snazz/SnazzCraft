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
#include <variant>

#include "external/glm/glm.hpp"

#include "snazzcraft-engine/entity/entity.hpp"
#include "snazzcraft-engine/voxel/voxel.hpp"
#include "snazzcraft-engine/entity/user.hpp"
#include "snazzcraft-engine/utilities/thread-pool-basic.hpp"

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

        World(std::string IName, uint32_t ISize, int32_t ISeed);

        ~World();

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

        bool DestroyVoxel(const glm::vec3& Position, const glm::vec3& Rotation);

        bool PlaceVoxel(const glm::vec3& Position, const glm::vec3& Rotation, uint8_t VoxelID);

        void UpdateVoxelPlacementDisplay();

        bool ChunkWithinWorld(SnazzCraft::Chunk* Chunk) const;
        
        void AddChunkUpdateTaskToThreadPool(SnazzCraft::Chunk* Chunk);

        inline void ApplyGravityToAllEntities() const;

        inline const glm::vec3& GetVoxelPlacementDisplayPosition() const; 

        inline bool ChunkWithinWorld(int32_t ChunkX, int32_t ChunkZ) const;

    private:
        SnazzCraft::ThreadPoolBasic<void> ThreadPool; // 6 threads

        std::unordered_map<uint64_t, SnazzCraft::Chunk*> Chunks; // Uses SnazzCraft::IntegerHash for hashing
        mutable std::recursive_mutex ChunksMutex;

        std::vector<SnazzCraft::Entity*> Entities;
        mutable std::recursive_mutex EntitiesMutex;

        SnazzCraft::HeightMap* HeightMap{};
        mutable std::recursive_mutex HeightMapMutex;

        SnazzCraft::Mesh* VoxelPlacementDisplayMesh{};
        glm::vec3 VoxelPlacementDisplayPosition;
        bool ShouldRenderVoxelPlacementDisplay{};

        void GenerateChunk(int32_t X, int32_t Z); 

        void RenderAllEntities() const;

        void RenderChunks() const;

        void RenderVoxelPlacementDisplay() const;

        /*
        Rotation is expected to not be normalized
        */
        bool RaycastToVoxel(glm::vec3& Position, const glm::vec3& Rotation, float MaxDistance, uint8_t* FaceHit, SnazzCraft::World::VoxelCollisionInfo* VoxelCollisionInfo); 

        void UpdateChunkLighting(SnazzCraft::Chunk* Chunk, bool* UpatedInputChunk);

        void ApplySunLightingToChunk(SnazzCraft::Chunk* Chunk, std::unordered_set<uint64_t>* ChunksToUpdate);

        void ApplySunLightingToColumn(SnazzCraft::Chunk* Chunk, uint32_t LocalChunkX, uint32_t LocalChunkZ, uint32_t StartY, int8_t StartLightValue, std::unordered_set<uint64_t>* ChunksToUpdate);
        
        /*
        Only to be called trough UpdateChunkLighting
        Generates currently ungenerated Chunks when light values would affect them
        */
        void ApplyLightingVoxel(int32_t LightOrigin[3], int8_t LightProducingLevel, std::unordered_set<uint64_t>* ChunksToUpdate);
        
    public:
        static SnazzCraft::World* CreateWorld(std::string Name, uint32_t Size, int32_t Seed);


    private:
        class LightNode;

        friend class SnazzCraft::Chunk;

    };
    
    extern SnazzCraft::World* Overworld;
} // SnazzCraft

#include "snazzcraft-engine/world/world.inl"
#include "snazzcraft-engine/world/voxel-collision-info.inl"
