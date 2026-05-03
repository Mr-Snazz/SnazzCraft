#include "snazzcraft-engine/world/world.hpp"
#include "snazzcraft-engine/core/core.hpp"
#include "snazzcraft-engine/utilities/math.hpp"
#include "snazzcraft-engine/texture/texture.hpp"
#include "snazzcraft-engine/mesh/mesh.hpp"
#include "snazzcraft-engine/world/chunk.hpp"
#include "snazzcraft-engine/shader/voxel-shader.hpp"

void SnazzCraft::World::Render() const
{
    const SnazzCraft::VoxelShader& VoxelShaderInstance = VoxelShader::GetInstance();
    VoxelShaderInstance.SetLightPosition(SnazzCraft::CurrentWorld->Entities[0]->Position, true);
    VoxelShaderInstance.SetViewPosition(SnazzCraft::Player->Position, false);

    SnazzCraft::ViewMatrix = glm::lookAt(SnazzCraft::Player->Position, SnazzCraft::Player->Position + SnazzCraft::Player->Front, glm::vec3(0.0, 1.0, 0.0));
    glUniformMatrix4fv(SnazzCraft::ViewLock, 1, GL_FALSE, glm::value_ptr(SnazzCraft::ViewMatrix));
    VoxelShaderInstance.SetViewMatrix(SnazzCraft::ViewMatrix, false);

    // Render voxels in chunks & voxel placement display
    SnazzCraft::VoxelTextureAtlas->BindTexture();
    
    SnazzCraft::ModelMatrix = glm::mat4(1.0f);
    VoxelShaderInstance.SetModelMatrix(SnazzCraft::ModelMatrix, false);
    this->RenderChunks();

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

    RenderEntity(SnazzCraft::Player);
    for (SnazzCraft::Entity* Entity : this->Entities) {
        RenderEntity(Entity);
    }
}

void SnazzCraft::World::RenderChunks() const
{ 
    int32_t PlayerChunkPosition[2];
    SnazzCraft::Chunk::GetChunkPosition(Player->Position, PlayerChunkPosition);

    for (int32_t X = PlayerChunkPosition[0] - static_cast<int32_t>(this->RenderDistance); X <= PlayerChunkPosition[0] + static_cast<int32_t>(this->RenderDistance); X++) {
    for (int32_t Z = PlayerChunkPosition[1] - static_cast<int32_t>(this->RenderDistance); Z <= PlayerChunkPosition[1] + static_cast<int32_t>(this->RenderDistance); Z++) {
        if (!this->ChunkWithinWorld(X, Z)) continue;

        auto ChunkIterator = this->Chunks.find(SnazzCraft::IntegerHash(X, Z));
        if (ChunkIterator == this->Chunks.end()) continue;

        if (ChunkIterator->second->ChunkMesh == nullptr) continue; 
        ChunkIterator->second->ChunkMesh->Draw();
    }
    }
}

void SnazzCraft::World::RenderVoxelPlacementDisplay() const
{
    if (!this->ShouldRenderVoxelPlacementDisplay) return;

    SnazzCraft::ModelMatrix = glm::translate(glm::mat4(1.0f), this->VoxelPlacementDisplayPosition);
    SnazzCraft::ModelMatrix = glm::scale(SnazzCraft::ModelMatrix, this->VoxelPlacementDisplayMesh->ScaleVector);
    glUniformMatrix4fv(SnazzCraft::ModelLock, 1, GL_FALSE, glm::value_ptr(SnazzCraft::ModelMatrix));
    this->VoxelPlacementDisplayMesh->Draw();
}

