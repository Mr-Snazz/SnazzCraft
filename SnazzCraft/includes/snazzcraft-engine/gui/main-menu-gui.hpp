#pragma once

#include "glad.h"
#include "glfw3.h"

#include "snazzcraft-engine/gui/gui.hpp"

namespace SnazzCraft
{
    class MainMenuGUI : public SnazzCraft::GUI
    {
    public:


        MainMenuGUI(unsigned int WindowWidth, unsigned int WindowHeight, GLFWwindow* Window);

        


    private:


    };

    extern SnazzCraft::MainMenuGUI* MenuGUI;
} // SnazzCraft




