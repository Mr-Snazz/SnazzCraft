#include <thread>
#include <queue>

#include "snazzcraft-engine/tick-system/tick-system.hpp"
#include "snazzcraft-engine/core/mode.hpp"

#include "snazzcraft-engine/world/world.hpp"
#include "external/glfw3.h"

std::queue<SnazzCraft::TickSystem::Notification*> Notifications;

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

        SnazzCraft::TickSystem::HandleNotifications();

        HandleRedstoneTick = !HandleRedstoneTick;
    }
}

void SnazzCraft::TickSystem::HandleNotifications()
{
    while (!Notifications.empty())
    {
        SnazzCraft::TickSystem::Notification* Notification = std::move(Notifications.front());
        Notifications.pop();

        switch (Notification->ID)
        {
            case SnazzCraft::TickSystem::Notification::NotificationID::VoxelEditNextToLiquid:
            {
                std::lock_guard<std::recursive_mutex> ChunksLock(SnazzCraft::Overworld->ChunksMutex);

                break;
            }
            
            default:
                break;
        }

        //EndLoop:
        delete Notification;
    }
}

void SnazzCraft::TickSystem::AddNotification(SnazzCraft::TickSystem::Notification* Notification)
{
    Notifications.push(Notification);
}



