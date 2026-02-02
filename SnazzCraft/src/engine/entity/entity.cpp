#include "entity.hpp"

SnazzCraft::Entity::Entity(glm::vec3 Position, glm::vec3 Rotation)
{
    this->Position = Position;
    this->Rotation = Rotation;

    this->Front = SnazzCraft::CalculateFrontVector(this->Rotation);
}

SnazzCraft::Entity::~Entity()
{
    delete this->EntityMesh;
}

void SnazzCraft::Entity::Move(const glm::vec3& Rotation, float Distance)
{
    this->Position += glm::vec3(Distance, Distance, Distance) * SnazzCraft::CalculateFrontVector(this->Rotation + Rotation);
}

void SnazzCraft::Entity::Rotate(const glm::vec3& Rotation)
{
    for (unsigned int I = 0; I < 3; I++) {
        this->Rotation[I] += Rotation[I];
        this->Rotation[I] = this->Rotation[I] >  180.0f ? this->Rotation[I] - 360.0f : this->Rotation[I];
        this->Rotation[I] = this->Rotation[I] < -180.0f ? this->Rotation[I] + 360.0f : this->Rotation[I];
    }

    this->Front = SnazzCraft::CalculateFrontVector(this->Rotation);
}