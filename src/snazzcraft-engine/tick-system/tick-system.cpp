#include <thread>

#include "external/glfw3.h"

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
    double PreviousTime = glfwGetTime();
    double Delta{};
    bool HandleRedstoneTick{};

    while (!SnazzCraft::ShouldCloseApplication())
    {
        while (SnazzCraft::GetUserMode() == SnazzCraft::UserMode::MainMenu) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            if (SnazzCraft::ShouldCloseApplication()) return;
        }

        double CurrentTime = glfwGetTime();
        Delta = CurrentTime - PreviousTime;

        if (Delta < 0.05) continue; // Execute onces every 0.05 seconds
        PreviousTime = CurrentTime;

        if (HandleRedstoneTick) { // Implement redstone

        }

        

        HandleRedstoneTick = !HandleRedstoneTick;
    }
}