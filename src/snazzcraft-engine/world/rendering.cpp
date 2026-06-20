#include "snazzcraft-engine/world/world.hpp"
#include "snazzcraft-engine/core/core.hpp"
#include "snazzcraft-engine/utilities/math.hpp"
#include "snazzcraft-engine/texture/texture.hpp"
#include "snazzcraft-engine/mesh/mesh.hpp"
#include "snazzcraft-engine/chunk/chunk.hpp"
#include "snazzcraft-engine/shader/voxel-shader.hpp"
#include "snazzcraft-engine/entity/entity-ids.h"

void SnazzCraft::World::Render() const
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
    if (SnazzCraft::WireframeModeActive) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    const SnazzCraft::VoxelShader& VoxelShaderInstance = VoxelShader::GetInstance();
    VoxelShaderInstance.SetLightPosition(SnazzCraft::Overworld->Entities[0]->Position, true);
    VoxelShaderInstance.SetViewPosition(SnazzCraft::Player->Position, false);

    SnazzCraft::ViewMatrix = glm::lookAt(SnazzCraft::Player->Position, SnazzCraft::Player->Position + SnazzCraft::Player->Front, glm::vec3(0.0, 1.0, 0.0));
    VoxelShaderInstance.SetViewMatrix(SnazzCraft::ViewMatrix, false);
    
    SnazzCraft::ModelMatrix = glm::mat4(1.0f);
    VoxelShaderInstance.SetModelMatrix(SnazzCraft::ModelMatrix, false);

    this->RenderChunks();

    SnazzCraft::Overworld->UpdateVoxelPlacementDisplay();
    this->RenderVoxelPlacementDisplay();
    
    this->RenderAllEntities();
}

void SnazzCraft::World::RenderAllEntities() const
{
    auto RenderEntity = [](SnazzCraft::Entity* Entity) -> void
    {
        const SnazzCraft::EntityType& EntityType = Entity->GetEntityType();

        SnazzCraft::ModelMatrix = glm::translate(glm::mat4(1.0f), Entity->Position);
        
        SnazzCraft::ModelMatrix = glm::rotate(SnazzCraft::ModelMatrix, glm::radians( Entity->Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        SnazzCraft::ModelMatrix = glm::rotate(SnazzCraft::ModelMatrix, glm::radians(-Entity->Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        SnazzCraft::ModelMatrix = glm::rotate(SnazzCraft::ModelMatrix, glm::radians( Entity->Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        SnazzCraft::ModelMatrix = glm::scale(SnazzCraft::ModelMatrix, EntityType.EntityMesh->ScaleVector);
        SnazzCraft::VoxelShader::GetInstance().SetModelMatrix(SnazzCraft::ModelMatrix, false);

        EntityType.EntityTexture->BindTexture();
        EntityType.EntityMesh->Draw();
    };

    std::lock_guard<std::recursive_mutex> EntitiesLock(this->EntitiesMutex);

    RenderEntity(SnazzCraft::Player);
    for (SnazzCraft::Entity* Entity : this->Entities) RenderEntity(Entity);
}

void SnazzCraft::World::RenderChunks() const
{ 
    int32_t PlayerChunkPosition[2];
    SnazzCraft::Chunk::GetChunkPosition(Player->Position, PlayerChunkPosition);

    for (int32_t X = PlayerChunkPosition[0] - static_cast<int32_t>(this->RenderDistance); X <= PlayerChunkPosition[0] + static_cast<int32_t>(this->RenderDistance); X++) {
    for (int32_t Z = PlayerChunkPosition[1] - static_cast<int32_t>(this->RenderDistance); Z <= PlayerChunkPosition[1] + static_cast<int32_t>(this->RenderDistance); Z++) {
        if (!this->ChunkWithinWorld(X, Z)) continue;

        uint64_t Hash = SnazzCraft::IntegerHash(X, Z);
        std::lock_guard<std::recursive_mutex> ChunksToUpdateMeshesLock(this->ChunksMutex);

        auto ChunkIterator = this->Chunks.find(Hash);
        if (ChunkIterator == this->Chunks.end()) continue;

        if (ChunkIterator->second->ShouldUpdateMesh) { ChunkIterator->second->UpdateMesh(); ChunkIterator->second->ShouldUpdateMesh = false; }

        if (!ChunkIterator->second->HasValidMesh()) continue; 
        ChunkIterator->second->Draw();
    }
    }
    
}

void SnazzCraft::World::RenderVoxelPlacementDisplay() const
{
    if (!this->ShouldRenderVoxelPlacementDisplay) return;

    SnazzCraft::ModelMatrix = glm::translate(glm::mat4(1.0f), this->VoxelPlacementDisplayPosition);
    SnazzCraft::ModelMatrix = glm::scale(SnazzCraft::ModelMatrix, this->VoxelPlacementDisplayMesh->ScaleVector);
    SnazzCraft::VoxelShader::GetInstance().SetModelMatrix(SnazzCraft::ModelMatrix, false);

    this->VoxelPlacementDisplayMesh->Draw();
}

