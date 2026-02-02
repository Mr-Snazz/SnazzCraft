#pragma once

#include <vector>

#include "../../../../includes/glfw3.h"

#include "event/event.hpp"
#include "event/keys.h"
#include "event/events.h"
#include "event/data/data-types.h"

namespace SnazzCraft
{
    class InputHandler
    {
    public:
        std::vector<SnazzCraft::Event*> EventQueue;
        void (*Callback)(SnazzCraft::Event*) = nullptr;

        InputHandler(GLFWwindow* Window);

        ~InputHandler();

        virtual void PollEvents();

        virtual void HandleEvents(); // Does not clear queue if no callback is set

    private:
        GLFWwindow* Window;
        

    };
}
