#pragma once

#include <vector>
#include <stdint.h>
#include <fstream>
#include <sstream>
#include <string>

#include "external/glm/glm.hpp"
#include "external/glad.h"
#include "external/shader_s.h"

#include "snazzcraft-engine/mesh/voxel-vertice.hpp"
#include "snazzcraft-engine/mesh/mesh-base.hpp"

namespace SnazzCraft
{
    class ChunkMesh : public SnazzCraft::MeshBase
    {
    public:
        std::vector<SnazzCraft::VoxelVertice> Vertices;
        std::vector<uint32_t> Indices;

        ChunkMesh(const std::vector<SnazzCraft::VoxelVertice>& IVertices, const std::vector<uint32_t>& IIndices, bool Initiate); // Use shader before constructing

        virtual ~ChunkMesh();

        virtual void Draw() const;

        virtual void UpdateGPUData(bool BindVAO, bool UnbindPostUpdate);

    protected:
        virtual void Initiate();
    };
}

