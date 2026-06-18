#pragma once

#include "external/shader_s.h"
#include "external/glm/glm.hpp"
#include "external/glm/gtc/matrix_transform.hpp"
#include "external/glm/gtc/type_ptr.hpp"

namespace SnazzCraft
{
    class GUIShader : public LearnOpenGLShader
    {
    public:
        GUIShader(const char* vertexPath, const char* fragmentPath);

        virtual ~GUIShader() = default;

        inline void SetOrthographicProjectionMatrix(const glm::mat4& OrthographicProjectionMatrix) const;

    protected:

    private:


    };
}

#include "snazzcraft-engine/shader/gui-shader.inl"

