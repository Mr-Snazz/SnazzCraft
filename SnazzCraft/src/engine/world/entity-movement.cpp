#include "snazzcraft-engine/world/world.hpp"

void SnazzCraft::World::MoveEntity(SnazzCraft::Entity* Entity, const glm::vec3& Rotation, float Distance) const
{
    glm::vec3 NewPosition = Entity->Position;
    MoveVector3D(NewPosition, Entity->Rotation + Rotation, Distance);

    for (uint32_t I = 0; I < 3; I++) {
        float OldCoordinate = Entity->Position[I];

        Entity->Position[I] = NewPosition[I];
        Entity->EntityHitbox->Position[I] = NewPosition[I];

        SnazzCraft::Voxel* CollisionVoxel = this->GetCollidingVoxel(Entity->EntityHitbox);
        if (CollisionVoxel == nullptr) continue;

        Entity->Position[I] = OldCoordinate;
        Entity->EntityHitbox->Position[I] = OldCoordinate;
    }
}

void SnazzCraft::World::MoveEntity(glm::vec3 Translation, SnazzCraft::Entity* Entity) const
{
    for (uint32_t I = 0; I < 3; I++) {
        float OldCoordinate = Entity->Position[I];

        Entity->Position[I] += Translation[I];
        Entity->EntityHitbox->Position[I] = Entity->Position[I];

        SnazzCraft::Voxel* CollisionVoxel = this->GetCollidingVoxel(Entity->EntityHitbox);
        if (CollisionVoxel == nullptr) continue;

        Entity->Position[I] = OldCoordinate;
        Entity->EntityHitbox->Position[I] = OldCoordinate;
    }
}