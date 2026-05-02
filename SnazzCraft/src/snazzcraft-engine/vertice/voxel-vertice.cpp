#include "snazzcraft-engine/vertice/voxel-vertice.hpp"

SnazzCraft::VoxelVertice::VoxelVertice()
    : Position(glm::vec3(0.0f)), Normal(glm::vec3(0.0f)), TextureCoordinate(glm::vec2(0.0f)), Brightness(1.0f)
{

}

SnazzCraft::VoxelVertice::VoxelVertice(glm::vec3 IPosition, glm::vec3 INormal, glm::vec2 ITextureCoordinate) 
    : Position(IPosition), Normal(INormal), TextureCoordinate(ITextureCoordinate), Brightness(1.0f)
{ 

}

SnazzCraft::VoxelVertice::VoxelVertice(glm::vec3 IPosition, glm::vec3 INormal, glm::vec2 ITextureCoordinate, float IBrightness)
    : Position(IPosition), Normal(INormal), TextureCoordinate(ITextureCoordinate), Brightness(IBrightness)
{

}