#pragma once

#include <vector>
#include <stdint.h>

#include "external/glm/glm.hpp"
#include "external/glad.h"
#include "external/shader_s.h"

namespace SnazzCraft
{
    class MeshBase
    {
    public:
        virtual ~MeshBase();

        virtual void Draw() const;

        virtual void UpdateGPUData(bool BindVAO, bool UnbindPostUpdate);

        inline bool IsValid() const;

    protected:
        uint32_t VAO;
        uint32_t VBO;
        uint32_t EBO;
        bool ShouldInitiate;

        MeshBase(bool Initiate);

        virtual void Initiate();
    };
}

#include "snazzcraft-engine/mesh/mesh-base.inl"

