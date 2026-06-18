#pragma once

#include <stdint.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

namespace SnazzCraft
{
    class Mesh;
    class Texture;
    class Hitbox;

    class EntityType
    {
    public:
        SnazzCraft::Mesh* EntityMesh;
        SnazzCraft::Texture* EntityTexture;
        SnazzCraft::Hitbox* EntityHitbox;

        static void Initialize();

        static void FreeResources();
        
        static const EntityType& GetEntityType(uint8_t ID);

    private:
        static SnazzCraft::Mesh* Meshes[256];
        static uint8_t MeshesLoaded;

        static SnazzCraft::Texture* Textures[256];
        static uint8_t TexturesLoaded;

        static SnazzCraft::Hitbox* Hitboxes[256];
        static uint8_t HitboxesLoaded;

        EntityType(SnazzCraft::Mesh* IEntityMesh, SnazzCraft::Texture* IEntityTexture, SnazzCraft::Hitbox* IEntityHitbox);

        static void LoadMeshes();

        static void LoadTextures();

        static void LoadHitboxes();

    };


} // SnazzCraft