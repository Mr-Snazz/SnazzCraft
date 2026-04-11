#include "snazzcraft-engine/gui/main-menu-gui.hpp"
#include "snazzcraft-engine/gui/panel/panel-with-texture.hpp"
#include "snazzcraft-engine/input-handler/core-callback.hpp"
#include "snazzcraft-engine/core/window.hpp"
#include "snazzcraft-engine/gui/panel/panel-with-text-texture.hpp"

SnazzCraft::MainMenuGUI::MainMenuGUI()
    : GUI()
{
    
}

void SnazzCraft::MainMenuGUI::Initialize(void(*InputHandlerCallback)(SnazzCraft::Event* Event))
{
    MainMenuGUI& Instance = SnazzCraft::MainMenuGUI::GetInstance();

    // Backround
    SnazzCraft::PanelWithTexture* NewPanelWithTexture = new SnazzCraft::PanelWithTexture(0.0f, 0.0f, 900.0f, 900.0f, 1.0f, "textures/voxel/individuals/dirt.png");
    Instance.Panels.push_back(NewPanelWithTexture);

    // Load World
    SnazzCraft::TextTexture* LoadWorldTextTexture = new SnazzCraft::TextTexture("Load World", 0xFF, 0xFF, 0xFF);

    NewPanelWithTexture = new SnazzCraft::PanelWithTexture(10.0f, 10.0f, LoadWorldTextTexture->Width, LoadWorldTextTexture->Height, 3.0f, "textures/gui/button.png");
    Instance.Panels.push_back(NewPanelWithTexture);

    SnazzCraft::PanelWithTextTexture* NewPanelWithTextTexture = new SnazzCraft::PanelWithTextTexture(10.0f, 10.0f, 3.0f, LoadWorldTextTexture);
    NewPanelWithTextTexture->SetCallback(SnazzCraft::CreateWorldAndChangeModeToWorld);
    Instance.Panels.push_back(NewPanelWithTextTexture);


    Instance.SetInputHandler(SnazzCraft::Window);
    Instance.SetInputHandlerCallback(InputHandlerCallback);
}
