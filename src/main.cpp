#include "snazzcraft-engine/core/core.hpp"

int main(int ArgC, char* ArgV[])
{ 
    SnazzCraft::Initiate();
    SnazzCraft::MainLoop();
    SnazzCraft::FreeResources();
    
    return 0;
}
