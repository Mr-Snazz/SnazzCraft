#pragma once

#include "../../../../includes/glm/glm.hpp"

namespace SnazzCraft
{
	struct Vertice3D
	{
		glm::vec3 Position;
		glm::vec2 TextureCoordinate;

		Vertice3D();

		Vertice3D(glm::vec3 Position, glm::vec2 TextureCoordinate);
	};
}
