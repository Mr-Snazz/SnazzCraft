#include "snazzcraft-engine/mesh/chunk-mesh.hpp"

SnazzCraft::ChunkMesh::ChunkMesh(const std::vector<SnazzCraft::VoxelVertice>& IVertices, const std::vector<uint32_t>& IIndices, bool Initiate)
    : MeshBase(Initiate), Vertices(IVertices), Indices(IIndices)
{
    if (Initiate) this->Initiate();
}

SnazzCraft::ChunkMesh::~ChunkMesh()
{
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
}

void SnazzCraft::ChunkMesh::Draw() const
{
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->Indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void SnazzCraft::ChunkMesh::UpdateGPUData(bool BindVAO, bool UnbindPostUpdate)
{
    if (this->ShouldInitiate) { this->ShouldInitiate = false; this->Initiate(); return; }

    if (BindVAO) glBindVertexArray(this->VAO);

    // Update VBO with current vertex data
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->Vertices.size() * sizeof(SnazzCraft::VoxelVertice), this->Vertices.data(), GL_DYNAMIC_DRAW);

    // Update EBO with current index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->Indices.size() * sizeof(uint32_t), this->Indices.data(), GL_DYNAMIC_DRAW);

    if (!UnbindPostUpdate) return;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SnazzCraft::ChunkMesh::Initiate()
{
    if (this->VAO != 0u) glDeleteVertexArrays(1, &this->VAO);
    if (this->VBO != 0u) glDeleteBuffers(1, &this->VBO);
    if (this->EBO != 0u) glDeleteBuffers(1, &this->EBO);

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);

    this->UpdateGPUData(false, false); 

    // Set vertex attribute pointers

    // layout (location = 0) vec3 position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SnazzCraft::VoxelVertice), (void*)0);
    glEnableVertexAttribArray(0);

    // layout (location = 1) vec3 normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SnazzCraft::VoxelVertice), (void*)offsetof(SnazzCraft::VoxelVertice, SnazzCraft::VoxelVertice::Normal));
    glEnableVertexAttribArray(1);

    // layout (location = 2) vec2 texCoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SnazzCraft::VoxelVertice), (void*)offsetof(SnazzCraft::VoxelVertice, SnazzCraft::VoxelVertice::TextureCoordinate));
    glEnableVertexAttribArray(2);

    // layout (location = 3) float brightness
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(SnazzCraft::VoxelVertice), (void*)offsetof(SnazzCraft::VoxelVertice, SnazzCraft::VoxelVertice::Brightness));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

