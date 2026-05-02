#pragma once

#include <stdint.h>

#include "external/shader_s.h"
#include "external/glm/glm.hpp"
#include "external/glm/gtc/matrix_transform.hpp"
#include "external/glm/gtc/type_ptr.hpp"

namespace SnazzCraft
{
    class VoxelShader : public LearnOpenGLShader
    {
    public:
        virtual ~VoxelShader() = default;

        // VoxelShader calls this->Use() before setting any uniform
        inline void SetModelMatrix(const glm::mat4& ModelMatrix, bool CallUse) const;

        inline void SetViewMatrix(const glm::mat4& ViewMatrix, bool CallUse) const;

        inline void SetProjectionMatrix(const glm::mat4& ProjectionMatrix, bool CallUse) const;

        inline void SetLightPosition(const glm::vec3& LightPosition, bool CallUse) const;

        inline void SetViewPosition(const glm::vec3& ViewPosition, bool CallUse) const;

        inline void SetAmbient(float Ambient, bool CallUse) const;

    protected:


    private:
        VoxelShader(const char* vertexPath, const char* fragmentPath);


    public:
        static inline const VoxelShader& GetInstance();

    };
}

#include "snazzcraft-engine/shader/voxel-shader.inl"


