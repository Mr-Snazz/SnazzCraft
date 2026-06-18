#include "snazzcraft-engine/mesh/mesh.hpp"

inline bool SnazzCraft::Mesh::IsValid() const
{
    return !this->Vertices.empty() && !this->Indices.empty() && this->VAO != 0u && this->VBO != 0u && this->EBO != 0u;
}

