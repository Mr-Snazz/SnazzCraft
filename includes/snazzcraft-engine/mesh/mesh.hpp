#pragma once

#include <vector>
#include <stdint.h>
#include <fstream>
#include <sstream>
#include <string>

#include "external/glm/glm.hpp"
#include "external/glad.h"
#include "external/shader_s.h"

#include "snazzcraft-engine/mesh/vertice.hpp"
#include "snazzcraft-engine/mesh/mesh-base.hpp"

namespace SnazzCraft
{
    class Mesh : public SnazzCraft::MeshBase
    {
    public:
        std::vector<SnazzCraft::Vertice> Vertices;
        std::vector<uint32_t> Indices;
        glm::vec3 ScaleVector;

        Mesh(const std::vector<SnazzCraft::Vertice>& IVertices, const std::vector<uint32_t>& IIndices, bool Initiate); // Use shader before constructing

        virtual ~Mesh();

        virtual void Draw() const;

        virtual void UpdateGPUData(bool BindVAO, bool UnbindPostUpdate);

    protected:
        virtual void Initiate();

    public:
        static SnazzCraft::Mesh* LoadMeshFromObjectFile(const char* FilePath, bool InitiateMesh);

    };

    extern SnazzCraft::Mesh* VoxelMesh;
} // SnazzCraft

