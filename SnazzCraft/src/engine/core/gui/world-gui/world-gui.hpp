#pragma once

#include "../gui.hpp"
#include "../../input-handler/core-callback/core-callback.hpp"

namespace SnazzCraft
{
    class InWorldGUI : public SnazzCraft::GUI
    {
    public:

        InWorldGUI(unsigned int WindowWidth, unsigned int WindowHeight, GLFWwindow* Window);

    private:


    };


    extern SnazzCraft::InWorldGUI* WorldGUI;
}

