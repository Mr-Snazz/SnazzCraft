#include <thread>
#include <queue>

#include "snazzcraft-engine/tick-system/tick-system.hpp"
#include "snazzcraft-engine/core/mode.hpp"
#include "snazzcraft-engine/voxel/liquid-type.hpp"

#include "snazzcraft-engine/world/world.hpp"
#include "external/glfw3.h"

void MainLoop();

void HandleNotifications();

void SnazzCraft::TickSystem::Initialize()
{
    std::thread MainLoopThread(MainLoop);
    MainLoopThread.detach();
}

void MainLoop()
{
    double PreviousTime = glfwGetTime();
    double Delta{};
    bool HandleRedstoneTick{};

    while (!SnazzCraft::ShouldCloseApplication())
    {
        while (SnazzCraft::GetUserMode() == SnazzCraft::UserMode::MainMenu) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10u));
            if (SnazzCraft::ShouldCloseApplication()) return;
        }

        double CurrentTime = glfwGetTime();
        Delta = CurrentTime - PreviousTime;

        if (Delta < 0.05) continue; // Execute onces every 0.05 seconds
        PreviousTime = CurrentTime;

        

        HandleRedstoneTick = !HandleRedstoneTick;
    }
}




