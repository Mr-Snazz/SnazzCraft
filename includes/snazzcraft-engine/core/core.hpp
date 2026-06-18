#pragma once

#include <vector>
#include <stdint.h>

#include "external/glad.h"
#include "external/glfw3.h"
#include "external/shader_s.h"
#include "external/glm/glm.hpp"
#include "external/glm/gtc/matrix_transform.hpp"
#include "external/glm/gtc/type_ptr.hpp"

#include "snazzcraft-engine/gui/main-menu-gui.hpp"
#include "snazzcraft-engine/gui/world-gui.hpp"
#include "snazzcraft-engine/input-handler/events.h"
#include "snazzcraft-engine/input-handler/event.hpp"
#include "snazzcraft-engine/input-handler/data-types.h"
#include "snazzcraft-engine/core/mode.hpp"
#include "snazzcraft-engine/input-handler/input-handler.hpp"

#ifdef _WIN32
    #define CLEAR_COMMAND "cls"
#endif // _WIN32

#ifdef __linux__
    #define CLEAR_COMMAND "clear"
#endif // __linux__

namespace SnazzCraft
{
    class World;

    extern glm::mat4 ProjectionMatrix;
    extern glm::mat4 ModelMatrix;
    extern glm::mat4 ViewMatrix;

    extern bool CloseApplication;

    extern World* CurrentWorld;

    bool Initiate();

    void MainLoop();

    void FreeResources();
} // SnazzCraft

inline void FrameBufferSizeCallBack(GLFWwindow* Window, int Width, int Height);

inline void MouseButtonCallback(GLFWwindow* Window, int Button, int Action, int Mods);

#include "snazzcraft-engine/core/frame-buffer-size-callback.inl"
#include "snazzcraft-engine/core/mouse-button-callback.inl"
