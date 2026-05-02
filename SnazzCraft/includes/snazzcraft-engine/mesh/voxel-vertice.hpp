#pragma once

#include "external/glm/glm.hpp"

namespace SnazzCraft
{
	class VoxelVertice 
	{
	public:
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TextureCoordinate;
		float Brightness = 1.0f;

		VoxelVertice();

		VoxelVertice(glm::vec3 IPosition, glm::vec3 INormal, glm::vec2 ITextureCoordinate);

		VoxelVertice(glm::vec3 IPosition, glm::vec3 INormal, glm::vec2 ITextureCoordinate, float IBrightness);

	private:

	
	};
} // SnazzCraft
