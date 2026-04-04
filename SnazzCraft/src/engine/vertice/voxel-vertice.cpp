#include "snazzcraft-engine/vertice/voxel-vertice.hpp"

SnazzCraft::VoxelVertice::VoxelVertice()
{

}

SnazzCraft::VoxelVertice::VoxelVertice(glm::vec3 Position, glm::vec2 TextureCoordinate) 
{ 
    this->Position = Position; 
    this->TextureCoordinate = TextureCoordinate; 
}

SnazzCraft::VoxelVertice::VoxelVertice(glm::vec3 Position, glm::vec2 TextureCoordinate, float Brightness)
{
    this->Position = Position;
    this->TextureCoordinate = TextureCoordinate;
    this->Brightness = Brightness;
}