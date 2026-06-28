#include <thread>

#include "snazzcraft-engine/tick-system/tick-system.hpp"
#include "snazzcraft-engine/core/mode.hpp"

void MainLoop();

void SnazzCraft::TickSystem::Initialize()
{
    std::thread MainLoopThread(MainLoop);
    MainLoopThread.detach();
}

void MainLoop()
{
    while (!SnazzCraft::ShouldCloseApplication())
    {
        
    }
}