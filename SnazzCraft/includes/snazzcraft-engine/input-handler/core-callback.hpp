#pragma once

#include "glad.h"
#include "glfw3.h"

namespace SnazzCraft
{
    class Event;

    void ChangeModeCallback(SnazzCraft::Event* Event);

    void CloseWindowCallback(SnazzCraft::Event* Event);

    void ToggleWireframeCallback(SnazzCraft::Event* Event);
} // SnazzCraft


