#include "snazzcraft-engine/world/world.hpp"
#include "snazzcraft-engine/core/core.hpp"
#include "snazzcraft-engine/utilities/math.hpp"
#include "snazzcraft-engine/texture/texture.hpp"
#include "snazzcraft-engine/mesh/mesh.hpp"
#include "snazzcraft-engine/chunk/chunk.hpp"
#include "snazzcraft-engine/shader/voxel-shader.hpp"
#include "snazzcraft-engine/shader/entity-shader.hpp"
#include "snazzcraft-engine/entity/entity-ids.h"

void SnazzCraft::World::Render() 
{   
    if (SnazzCraft::Overworld->Entities.size() == 0) {
        std::lock_guard<std::recursive_mutex> EntitieLock(this->EntitiesMutex);
        SnazzCraft::Overworld->Entities.push_back(new SnazzCraft::Entity(glm::vec3(0.0f, static_cast<float>(SnazzCraft::Chunk::OceanLevel * 2 + 40), 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), ID_ENTITY_TEST));
    } else {
        //SnazzCraft::Overworld->MoveEntity(SnazzCraft::Overworld->Entities[0], glm::vec3(0.0f), 0.01f);

        SnazzCraft::Overworld->Entities[0]->Rotation.x += 0.5f;
        SnazzCraft::Overworld->Entities[0]->Rotation.y += 1.0f;
        SnazzCraft::Overworld->Entities[0]->Rotation.z += 1.5f;
    }

    if (!SnazzCraft::VoxelTextureAtlas->BindTexture()) return;

    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT); 
    glFrontFace(GL_CW);  
    glEnable(GL_CULL_FACE);
    if (SnazzCraft::WireframeModeActive()) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else                                 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    const glm::mat4 ViewMatrix = glm::lookAt(SnazzCraft::Player->Position, SnazzCraft::Player->Position + SnazzCraft::Player->Front, glm::vec3(0.0, 1.0, 0.0));

    const SnazzCraft::VoxelShader& VoxelShaderInstance = SnazzCraft::VoxelShader::GetInstance();
    VoxelShaderInstance.Use();
    VoxelShaderInstance.SetViewMatrix(ViewMatrix);

    this->RenderChunks();

    SnazzCraft::Overworld->UpdateVoxelPlacementDisplay();
    this->RenderVoxelPlacementDisplay();
    
    const SnazzCraft::EntityShader& EntityShaderInstance = SnazzCraft::EntityShader::GetInstance();
    EntityShaderInstance.Use();
    EntityShaderInstance.SetViewMatrix(ViewMatrix);
    this->RenderAllEntities();
}

void SnazzCraft::World::RenderAllEntities() const
{
    auto RenderEntity = [](SnazzCraft::Entity* Entity) -> void
    {
        const SnazzCraft::EntityType& EntityType = Entity->GetEntityType();

        glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), Entity->Position);
        
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians( Entity->Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-Entity->Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians( Entity->Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        ModelMatrix = glm::scale(ModelMatrix, EntityType.EntityMesh->ScaleVector);
        SnazzCraft::EntityShader::GetInstance().SetModelMatrix(ModelMatrix);

        EntityType.EntityTexture->BindTexture();
        EntityType.EntityMesh->Draw();
    };

    std::lock_guard<std::recursive_mutex> EntitiesLock(this->EntitiesMutex);

    RenderEntity(SnazzCraft::Player);
    for (SnazzCraft::Entity* Entity : this->Entities) RenderEntity(Entity);
}

void SnazzCraft::World::RenderChunks() 
{ 
    constexpr glm::vec3 VoxelSizeVector(SnazzCraft::Voxel::Size);

    int32_t PlayerChunkPosition[2];
    SnazzCraft::Chunk::GetChunkPosition(Player->Position / VoxelSizeVector, PlayerChunkPosition);
 
    std::lock_guard<std::recursive_mutex> ChunksToUpdateMeshesLock(this->ChunksMutex);
    for (int32_t X = PlayerChunkPosition[0] - static_cast<int32_t>(this->RenderDistance); X <= PlayerChunkPosition[0] + static_cast<int32_t>(this->RenderDistance); X++) {
    for (int32_t Z = PlayerChunkPosition[1] - static_cast<int32_t>(this->RenderDistance); Z <= PlayerChunkPosition[1] + static_cast<int32_t>(this->RenderDistance); Z++) {
        if (!this->ChunkWithinWorld(X, Z)) continue;

        uint64_t Hash = SnazzCraft::IntegerHash(X, Z);

        auto ChunkIterator = this->Chunks.find(Hash);
        if (ChunkIterator == this->Chunks.end()) {
            this->ThreadPool.Enqueue([this, X, Z](){ this->GenerateChunk(X, Z); });
            continue;
        }

        if (ChunkIterator->second->ShouldUpdateMesh) { ChunkIterator->second->UpdateMesh(); ChunkIterator->second->ShouldUpdateMesh = false; }

        if (!ChunkIterator->second->HasValidMesh()) continue; 

        glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), ChunkIterator->second->WorldOffset);
        SnazzCraft::VoxelShader::GetInstance().SetModelMatrix(ModelMatrix);
        
        ChunkIterator->second->Draw();
    }
    }
    
}

void SnazzCraft::World::RenderVoxelPlacementDisplay() const
{
    if (!this->ShouldRenderVoxelPlacementDisplay) return;

    glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), this->VoxelPlacementDisplayPosition);
    ModelMatrix = glm::scale(ModelMatrix, this->VoxelPlacementDisplayMesh->ScaleVector);
    SnazzCraft::VoxelShader::GetInstance().SetModelMatrix(ModelMatrix);

    this->VoxelPlacementDisplayMesh->Draw();
}

