#include <thread>
#include <queue>

#include "external/glfw3.h"

#include "snazzcraft-engine/tick-system/tick-system.hpp"
#include "snazzcraft-engine/core/mode.hpp"

std::queue<SnazzCraft::TickSystem::Notification*> Notifications;

void MainLoop();

void SnazzCraft::TickSystem::Initialize()
{
    std::thread MainLoopThread(MainLoop);
    MainLoopThread.detach();
}

void SnazzCraft::TickSystem::AddNotification(SnazzCraft::TickSystem::Notification* Notification)
{
    Notifications.push(Notification);
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

        if (HandleRedstoneTick) { // Implement redstone

        }

        

        HandleRedstoneTick = !HandleRedstoneTick;
    }
}