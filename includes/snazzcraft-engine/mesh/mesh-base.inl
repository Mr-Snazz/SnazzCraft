#include "snazzcraft-engine/mesh/mesh-base.hpp"

inline bool SnazzCraft::MeshBase::IsValid() const
{
    return this->VAO && this->VBO && this->EBO;
}
