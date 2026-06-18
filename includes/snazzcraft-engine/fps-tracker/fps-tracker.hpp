#pragma once

#include "external/glfw3.h"

namespace SnazzCraft
{
    namespace FPSTracker
    {
        void Initialize();

        void UpdateFPS();

        double GetDeltaTime();

        double GetFPS();
    }
} // SnazzCraft



