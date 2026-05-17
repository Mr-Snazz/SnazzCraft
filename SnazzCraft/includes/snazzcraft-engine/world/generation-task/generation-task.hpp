#pragma once

#include "snazzcraft-engine/world/world.hpp"

class SnazzCraft::World::GenerationTask
{
public:
    const uint16_t ID;

    virtual ~GenerationTask() = default;
    
    virtual void Execute() const;

protected:
    SnazzCraft::World* World;

    GenerationTask(uint16_t IID, SnazzCraft::World* IWorld);

private:
    


};
