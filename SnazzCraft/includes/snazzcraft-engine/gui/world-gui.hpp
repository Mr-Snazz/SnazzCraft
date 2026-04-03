#pragma once

#include "gui.hpp"
#include "../input-handler/core-callback.hpp"

#define SNAZZCRAFT_WORLD_GUI_HOTBAR_WIDGET_COLLECTION_INDEX    (0x01)
#define SNAZZCRAFT_WORLD_GUI_INVENTORY_WIDGET_COLLECTION_INDEX (0x02)

namespace SnazzCraft
{
    class InWorldGUI : public SnazzCraft::GUI
    {
    public:
        bool InInventory = false;

        InWorldGUI(unsigned int WindowWidth, unsigned int WindowHeight, GLFWwindow* Window); // Automatically sets AvtiveWidgetCollectionIndex

        inline void SwapMenus()
        {
            this->InInventory = !this->InInventory;

            this->ActiveWidgetCollectionIndex = (this->InInventory) ? SNAZZCRAFT_WORLD_GUI_INVENTORY_WIDGET_COLLECTION_INDEX : SNAZZCRAFT_WORLD_GUI_HOTBAR_WIDGET_COLLECTION_INDEX;
        }

    private:


    };


    extern SnazzCraft::InWorldGUI* WorldGUI;
} // SnazzCraft

