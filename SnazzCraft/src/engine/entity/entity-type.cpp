#include "snazzcraft-engine/entity/entity-type.hpp"

const char* TexturesToIntialize[] = {
    "textures/solids/light-green.png",
    "textures/solids/dark-blue.png",
    "textures/solids/white.png",
    nullptr
};
SnazzCraft::Texture* Textures[256];
uint8_t TexturesSet = 0x00;

const char* MeshesToInitialize[] = {
    "entity-meshes/test-cube.obj",
    "entity-meshes/test-rectangular-prism.obj",
    "entity-meshes/cattle.obj",
    nullptr
};
SnazzCraft::Mesh* Meshes[256];
uint8_t MeshesSet = 0x00;

const glm::vec3 HumanoidHitboxDimensions = glm::vec3(1.75f, 5.75f, 1.75f);
SnazzCraft::Hitbox* HumanoidHitbox;

const glm::vec3 CattleHitboxDimensions = glm::vec3(1.75f, 0.75f, 1.75f);
SnazzCraft::Hitbox* CattleHitbox;

void SnazzCraft::EntityType::Initialize()
{
    uint8_t TextureToIntializeIndex = 0x00;
    while (TexturesToIntialize[TextureToIntializeIndex] != nullptr)
    {
        Textures[TextureToIntializeIndex] = new SnazzCraft::Texture(TexturesToIntialize[TextureToIntializeIndex]);

        TextureToIntializeIndex++;
        TexturesSet++;
    }

    uint8_t MeshesToInitializeIndex = 0x00;
    while (MeshesToInitialize[MeshesToInitializeIndex] != nullptr)
    {
        Meshes[MeshesToInitializeIndex] = SnazzCraft::Mesh::LoadMeshFromObjectFile(MeshesToInitialize[MeshesToInitializeIndex]);

        MeshesToInitializeIndex++;
        MeshesSet++;
    }

    HumanoidHitbox = new SnazzCraft::Hitbox(HumanoidHitboxDimensions);
    CattleHitbox = new SnazzCraft::Hitbox(CattleHitboxDimensions);
    Meshes[2]->ScaleVector = glm::vec3(0.5f);
}

void SnazzCraft::EntityType::FreeResources()
{
    for (uint8_t I = 0x00; I < TexturesSet; I++) {
        delete Textures[I];
    }

    for (uint8_t I = 0x00; I < MeshesSet; I++) {
        delete Meshes[I];
    }

    delete HumanoidHitbox;
    delete CattleHitbox;
}

const SnazzCraft::EntityType& SnazzCraft::EntityType::GetEntityType(uint8_t ID)
{
    static const SnazzCraft::EntityType Player(Meshes[0], Textures[0], HumanoidHitbox);
    static const SnazzCraft::EntityType Test(Meshes[1], Textures[1], HumanoidHitbox);
    static const SnazzCraft::EntityType Sheep(Meshes[2], Textures[2], CattleHitbox);

    switch (ID)
    {
        case ID_ENTITY_PLAYER:
            return Player;

        case ID_ENTITY_TEST:
            return Test;

        case ID_ENTITY_SHEEP:
            return Sheep;

        default:
            return Test;
    }
}

SnazzCraft::EntityType::EntityType(SnazzCraft::Mesh* IEntityMesh, SnazzCraft::Texture* IEntityTexture, SnazzCraft::Hitbox* IEntityHitbox)
    : EntityMesh(IEntityMesh), EntityTexture(IEntityTexture), EntityHitbox(IEntityHitbox)
{

}