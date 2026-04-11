#include "snazzcraft-engine/gui/world-gui.hpp"
#include "snazzcraft-engine/core/window.hpp"

SnazzCraft::WorldGUI::WorldGUI()
    : GUI(), InInventory(false)
{

}

void SnazzCraft::WorldGUI::Initialize(void(*InputHandlerCallback)(SnazzCraft::Event* Event))
{
    SnazzCraft::WorldGUI& Instance = SnazzCraft::WorldGUI::GetInstance();

    Instance.SetInputHandler(SnazzCraft::Window);
    Instance.SetInputHandlerCallback(InputHandlerCallback);
}


