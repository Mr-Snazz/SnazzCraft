#include "snazzcraft-engine/shader/voxel-shader.hpp"

inline void SnazzCraft::VoxelShader::SetModelMatrix(const glm::mat4& ModelMatrix) const
{
    this->setMat4("Model", ModelMatrix);
}

inline void SnazzCraft::VoxelShader::SetViewMatrix(const glm::mat4& ViewMatrix) const
{
    this->setMat4("View", ViewMatrix);
}

inline void SnazzCraft::VoxelShader::SetProjectionMatrix(const glm::mat4& ProjectionMatrix) const
{
    this->setMat4("Projection", ProjectionMatrix);
}

inline const SnazzCraft::VoxelShader& SnazzCraft::VoxelShader::GetInstance()
{
    static VoxelShader Instance("src/glsl-shaders/voxel/voxel-vertex-shader.glsl", "src/glsl-shaders/voxel/voxel-fragment-shader.glsl");
    return Instance;
}

