#pragma once

#include "external/glm/glm.hpp"

namespace SnazzCraft
{
    class Vertice
    {
    public:
        glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TextureCoordinate;

        Vertice();

		Vertice(glm::vec3 IPosition, glm::vec3 INormal, glm::vec2 ITextureCoordinate);

    private:


    };
}