#include "snazzcraft-engine/shader/gui-shader.hpp"

inline void SnazzCraft::GUIShader::SetOrthographicProjectionMatrix(const glm::mat4& OrthographicProjectionMatrix) const
{
    this->setMat4("OrthographicProjection", OrthographicProjectionMatrix);
}
