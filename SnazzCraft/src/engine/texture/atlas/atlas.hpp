#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include "../../vertice/vertice-3d/vertice-3d.hpp"
#include "../../world/voxel/voxel.hpp"
#include "../../world/voxel/voxel-ids.h"
#include "../../mesh/mesh.hpp"

#define ATLAS_SIZE (5)
#define VOXEL_TEXTURE_SIZE (256)
#define ATLAS_SIZE_PIXELS (ATLAS_SIZE * VOXEL_TEXTURE_SIZE)
#define HALF_PIXEL (0.5f / (256 * ATLAS_SIZE))

namespace SnazzCraft
{
    class VoxelTextureApplier
    {
    public:
        VoxelTextureApplier(const char* AtlasFilePath);

        ~VoxelTextureApplier();

        std::vector<SnazzCraft::Vertice3D> GetTexturedVertices(const Voxel& Voxel);

    private:
        unsigned int TextureCoordinates[ATLAS_SIZE * ATLAS_SIZE][2];
        unsigned int TextureCoordinatesCount = 0;

        bool LoadAtlasCoordinates(const char* AtlasFilePath);
    };

    extern SnazzCraft::VoxelTextureApplier* EngineVoxelTextureApplier;
}

