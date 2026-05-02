#pragma once

#include "external/shader_s.h"

namespace SnazzCraft
{
    class VoxelShader : public LearnOpenGLShader
    {
    public:
        virtual ~VoxelShader() = default;

    protected:


    private:
        VoxelShader(const char* vertexPath, const char* fragmentPath);


    public:
        static inline const VoxelShader& GetInstance();

    };
}

#include "snazzcraft-engine/shader/voxel-shader.inl"


