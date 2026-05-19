#pragma once

#include <vector>
#include <stdint.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "external/glm/glm.hpp"
#include "external/glad.h"
#include "external/shader_s.h"

#include "snazzcraft-engine/mesh/voxel-vertice.hpp"

namespace SnazzCraft
{
    class Mesh
    {
    public:
        std::vector<SnazzCraft::VoxelVertice> Vertices;
        std::vector<uint32_t> Indices;
        glm::vec3 ScaleVector = { 1.0f, 1.0f, 1.0f };

        uint32_t VAO;

        Mesh(std::vector<SnazzCraft::VoxelVertice> Vertices, std::vector<uint32_t> Indices, bool Initiate); // Use shader before constructing

        virtual ~Mesh();

        virtual void Draw() const;

        void UpdateGPUData(bool BindVAO, bool UnbindPostUpdate);

        inline bool IsValid() const;

    private:
        uint32_t VBO;
        uint32_t EBO;
        bool ShouldInitiate;

        void Initiate();

    public:
        static SnazzCraft::Mesh* LoadMeshFromObjectFile(const char* FilePath);

    };

    extern SnazzCraft::Mesh* VoxelMesh;
} // SnazzCraft

#include "snazzcraft-engine/mesh/mesh.inl"