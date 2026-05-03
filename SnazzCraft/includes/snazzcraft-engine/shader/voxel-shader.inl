#include "snazzcraft-engine/shader/voxel-shader.hpp"

inline void SnazzCraft::VoxelShader::SetModelMatrix(const glm::mat4& ModelMatrix, bool CallUse) const
{
    if (CallUse) this->Use();
    this->setMat4("Model", ModelMatrix);
}

inline void SnazzCraft::VoxelShader::SetViewMatrix(const glm::mat4& ViewMatrix, bool CallUse) const
{
    if (CallUse) this->Use();
    this->setMat4("View", ViewMatrix);
}

inline void SnazzCraft::VoxelShader::SetProjectionMatrix(const glm::mat4& ProjectionMatrix, bool CallUse) const
{
    if (CallUse) this->Use();
    this->setMat4("Projection", ProjectionMatrix);
}

inline void SnazzCraft::VoxelShader::SetLightPosition(const glm::vec3& LightPosition, bool CallUse) const
{
    if (CallUse) this->Use();
    this->setVec3("LightPosition", LightPosition);
}

inline void SnazzCraft::VoxelShader::SetViewPosition(const glm::vec3& ViewPosition, bool CallUse) const
{
    if (CallUse) this->Use();
    this->setVec3("ViewPosition", ViewPosition);
}

inline void SnazzCraft::VoxelShader::SetAmbientStrength(float AmbientStrength, bool CallUse) const
{
    if (CallUse) this->Use();
    this->setFloat("AmbientStrength", AmbientStrength);
}

inline void SnazzCraft::VoxelShader::SetComplexLighting(bool Enabled, bool CallUse) const
{
    if (CallUse) this->Use();
    this->setBool("ComplexLightingEnabled", Enabled);
    this->ComplexLightingEnabled = Enabled;
}

inline bool SnazzCraft::VoxelShader::ComplexLightingIsEnabled() const
{
    return this->ComplexLightingEnabled;
}

inline const SnazzCraft::VoxelShader& SnazzCraft::VoxelShader::GetInstance()
{
    static VoxelShader Instance("src/glsl-shaders/voxel/voxel-vertex-shader.glsl", "src/glsl-shaders/voxel/voxel-fragment-shader.glsl");
    return Instance;
}

