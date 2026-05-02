#pragma once

#include "external/shader_s.h"

namespace SnazzCraft
{
    class GUIShader : public LearnOpenGLShader
    {
    public:
        GUIShader(const char* vertexPath, const char* fragmentPath);

        virtual ~GUIShader() = default;

    protected:

    private:


    };
}