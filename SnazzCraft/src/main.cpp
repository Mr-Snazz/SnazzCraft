#define STB_IMAGE_IMPLEMENTATION

#include "engine/core/core.hpp"

int main(int ArgC, char* ArgV[])
{ 
    if (!SnazzCraft::Initiate())
    {
        return -1;
    }

    SnazzCraft::MainLoop();

    SnazzCraft::FreeResources();
    
    return 0;
}