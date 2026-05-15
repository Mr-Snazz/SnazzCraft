#pragma once

#include <vector>
#include <stdint.h>

#include "external/glfw3.h"

#include "keys.h"
#include "events.h"
#include "data-types.h"

namespace SnazzCraft
{
    class Event;

    class InputHandler
    {
    public:
        std::vector<SnazzCraft::Event*> EventQueue;
        void(*Callback)(SnazzCraft::Event*) = nullptr;

        InputHandler(GLFWwindow* Window);

        virtual ~InputHandler();

        virtual void PollEvents();

        // Does not clear queue if no callback is set
        virtual void HandleEvents(); 

    private:
        GLFWwindow* Window;
        

    };
} // SnazzCraft
