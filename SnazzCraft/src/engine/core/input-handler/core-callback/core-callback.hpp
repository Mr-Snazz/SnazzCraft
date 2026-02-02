#pragma once

#include "../../../../../includes/glad.h"
#include "../../../../../includes/glfw3.h"

#include "../../mode/mode.hpp"
#include "../../../world/world.hpp"

namespace SnazzCraft
{
    void ChangeModeCallback(SnazzCraft::Event* Event);

    void CloseWindowCallback(SnazzCraft::Event* Event);

    void ToggleWireframeCallback(SnazzCraft::Event* Event);
}


