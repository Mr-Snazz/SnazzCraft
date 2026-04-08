#include <exception>

#include "snazzcraft-engine/core/core.hpp"
#include "snazzcraft-engine/world/world.hpp"

int main(int ArgC, char* ArgV[])
{ 
    if (!SnazzCraft::Initiate())
    {
        throw std::runtime_error("SnazzCraft failed to initiate.\n");
    }

    if (ArgC == 2) 
    {
        std::string WorldFilePath = "worlds/" + std::string(ArgV[1]) + ".txt";
        SnazzCraft::CurrentWorld = SnazzCraft::World::LoadWorldFromSaveFile(WorldFilePath);
    }

    SnazzCraft::MainLoop();

    SnazzCraft::FreeResources();
    
    return 0;
}