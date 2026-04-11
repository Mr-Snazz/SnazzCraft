#include "snazzcraft-engine/gui/main-menu-gui.hpp"
#include "snazzcraft-engine/gui/panel/panel-with-texture.hpp"
#include "snazzcraft-engine/input-handler/core-callback.hpp"
#include "snazzcraft-engine/core/window.hpp"

SnazzCraft::MainMenuGUI::MainMenuGUI()
    : GUI()
{
    
}

void SnazzCraft::MainMenuGUI::Initialize(void(*InputHandlerCallback)(SnazzCraft::Event* Event))
{
    MainMenuGUI& Instance = SnazzCraft::MainMenuGUI::GetInstance();

    Instance.Panels.push_back(new SnazzCraft::PanelWithTexture(0u, 0u, 900u, 900u, "textures/voxel/individuals/dirt.png"));
    Instance.Panels[0]->SetCallback(SnazzCraft::CreateWorldAndChangeModeToWorld);

    Instance.SetInputHandler(SnazzCraft::Window);
    Instance.SetInputHandlerCallback(InputHandlerCallback);
}
