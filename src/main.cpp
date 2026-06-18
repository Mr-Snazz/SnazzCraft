#include <exception>

#include "snazzcraft-engine/core/core.hpp"
#include "snazzcraft-engine/world/world.hpp"

int main(int ArgC, char* ArgV[])
{ 
    if (!SnazzCraft::Initiate())
    {
        throw std::runtime_error("SnazzCraft failed to initiate.\n");
    }

    SnazzCraft::MainLoop();

    SnazzCraft::FreeResources();

    return 0;
}