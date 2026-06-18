#pragma once

#include "snazzcraft-engine/gui/gui.hpp"

namespace SnazzCraft
{
    class Event;

    class WorldGUI : public GUI
    {
    public:
        bool InInventory;

        ~WorldGUI() = default;

    protected:
        WorldGUI();

    private:

    
    public:
        static void Initialize(void(*InputHandlerCallback)(SnazzCraft::Event* Event)); // Only to be called after SnazzCraft::Window is set to the memory address of a created window

        static inline WorldGUI& GetInstance()
        {
            static WorldGUI Instance;
            return Instance;
        }

        

    };

} // SnazzCraft