#include "snazzcraft-engine/world/generation-task/generation-task.hpp"

void SnazzCraft::World::GenerationTask::Execute() const
{
    
}

SnazzCraft::World::GenerationTask::GenerationTask(uint16_t IID, SnazzCraft::World* IWorld)
    : ID(IID), World(IWorld)
{

}