#include "snazzcraft-engine/mesh/mesh-base.hpp"

SnazzCraft::MeshBase::~MeshBase()
{
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
}

void SnazzCraft::MeshBase::Draw() const
{

}

void SnazzCraft::MeshBase::UpdateGPUData(bool BindVAO, bool UnbindPostUpdate)
{

}

SnazzCraft::MeshBase::MeshBase(bool Initiate)
    : VAO(0u), VBO(0u), EBO(0u), ShouldInitiate(!Initiate)
{

}

void SnazzCraft::MeshBase::Initiate()
{

}

