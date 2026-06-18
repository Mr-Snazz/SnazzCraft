#include <stdint.h>

#include "snazzcraft-engine/fps-tracker/fps-tracker.hpp"

uint32_t FrameCount{};
double FPS{};
double LastTime{};
double CurrentTime{};

void SnazzCraft::FPSTracker::Initialize()
{
    LastTime = glfwGetTime();
}

void SnazzCraft::FPSTracker::UpdateFPS()
{
    FrameCount++;
    CurrentTime = glfwGetTime();
    
    if (CurrentTime - LastTime >= 1.0) {
        FPS = static_cast<double>(FrameCount) / (CurrentTime - LastTime);
        FrameCount = 0;
        LastTime = CurrentTime;
    }
}

double SnazzCraft::FPSTracker::GetDeltaTime()
{
    return CurrentTime - LastTime;
}

double SnazzCraft::FPSTracker::GetFPS()
{
    return FPS;
}
