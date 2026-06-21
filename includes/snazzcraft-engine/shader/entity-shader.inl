#include "snazzcraft-engine/shader/entity-shader.hpp"

inline void SnazzCraft::EntityShader::SetModelMatrix(const glm::mat4& ModelMatrix, bool CallUse) const
{
    if (CallUse) this->Use();
    this->setMat4("Model", ModelMatrix);
}

inline void SnazzCraft::EntityShader::SetViewMatrix(const glm::mat4& ViewMatrix, bool CallUse) const
{
    if (CallUse) this->Use();
    this->setMat4("View", ViewMatrix);
}

inline void SnazzCraft::EntityShader::SetProjectionMatrix(const glm::mat4& ProjectionMatrix, bool CallUse) const
{
    if (CallUse) this->Use();
    this->setMat4("Projection", ProjectionMatrix);
}


inline const SnazzCraft::EntityShader& SnazzCraft::EntityShader::GetInstance()
{
    static EntityShader Instance("src/glsl-shaders/entity/entity-vertex-shader.glsl", "src/glsl-shaders/entity/entity-fragment-shader.glsl");
    return Instance;
}