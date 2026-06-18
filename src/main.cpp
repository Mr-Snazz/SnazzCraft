#include <exception>

#include "snazzcraft-engine/core/core.hpp"
#include "snazzcraft-engine/world/world.hpp"

int main(int ArgC, char* ArgV[])
{ 
    SnazzCraft::Initiate();
    SnazzCraft::MainLoop();
    SnazzCraft::FreeResources();

    return 0;
}