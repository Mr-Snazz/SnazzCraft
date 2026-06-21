#include "snazzcraft-engine/shader/entity-shader.hpp"

#include "snazzcraft-engine/shader/voxel-shader.hpp"

SnazzCraft::EntityShader::EntityShader(const char* vertexPath, const char* fragmentPath)
    : LearnOpenGLShader(vertexPath, fragmentPath)
{

}

