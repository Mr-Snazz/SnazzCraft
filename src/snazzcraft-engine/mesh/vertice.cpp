#include "snazzcraft-engine/mesh/vertice.hpp"

SnazzCraft::Vertice::Vertice()
    : Position(glm::vec3(0.0f)), Normal(glm::vec3(0.0f)), TextureCoordinate(glm::vec2(0.0f))
{

}

SnazzCraft::Vertice::Vertice(glm::vec3 IPosition, glm::vec3 INormal, glm::vec2 ITextureCoordinate) 
    : Position(IPosition), Normal(INormal), TextureCoordinate(ITextureCoordinate)
{ 

}
