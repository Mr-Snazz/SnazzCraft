#include "snazzcraft-engine/gui/world-gui.hpp"
#include "snazzcraft-engine/core/window.hpp"
#include "snazzcraft-engine/gui/panel/panel-with-text-texture.hpp"
#include "snazzcraft-engine/gui/panel/panel-with-texture.hpp"
#include "snazzcraft-engine/core/core-callback.hpp"

SnazzCraft::WorldGUI::WorldGUI()
    : GUI(), InInventory(false)
{

}

void SnazzCraft::WorldGUI::Initialize(void(*InputHandlerCallback)(SnazzCraft::Event* Event))
{
    SnazzCraft::WorldGUI& Instance = SnazzCraft::WorldGUI::GetInstance();

    // Back to main menu
    SnazzCraft::TextTexture* BackToMainMenuTextTexture = new SnazzCraft::TextTexture{ "Back to Main Menu", 0xFF, 0xFF, 0xFF };

    SnazzCraft::PanelWithTexture* BackToMainMenuPanelWithTexture = new SnazzCraft::PanelWithTexture(10.0f, 10.0f, BackToMainMenuTextTexture->Width, BackToMainMenuTextTexture->Height, 3.0f, "textures/gui/button.png");
    BackToMainMenuPanelWithTexture->SetCallback(SnazzCraft::ChangeModeToMainMenuCallback);
    Instance.Panels.push_back(BackToMainMenuPanelWithTexture);

    SnazzCraft::PanelWithTextTexture* BackToMainMenuPanelWithTextTexture = new SnazzCraft::PanelWithTextTexture{ 10.0f, 10.0f, 3.0f, BackToMainMenuTextTexture };
    Instance.Panels.push_back(BackToMainMenuPanelWithTextTexture);

    // Toggle wireframe
    SnazzCraft::TextTexture* ToggleWireframeTextTexture = new SnazzCraft::TextTexture{ "Toggle Wireframe", 0xFF, 0xFF, 0xFF };

    SnazzCraft::PanelWithTexture* ToggleWireframePanelWithTexture = new SnazzCraft::PanelWithTexture(10.0f, BackToMainMenuPanelWithTexture->GetY() + BackToMainMenuPanelWithTexture->GetHeight() + 10.0f, ToggleWireframeTextTexture->Width, ToggleWireframeTextTexture->Height, 3.0f, "textures/gui/button.png");
    ToggleWireframePanelWithTexture->SetCallback(SnazzCraft::ToggleWireframeCallback);
    Instance.Panels.push_back(ToggleWireframePanelWithTexture);

    SnazzCraft::PanelWithTextTexture* ToggleWireframePanelWithTextTexture = new SnazzCraft::PanelWithTextTexture{ 10.0f, BackToMainMenuPanelWithTexture->GetY() + BackToMainMenuPanelWithTexture->GetHeight() + 10.0f, 3.0f, ToggleWireframeTextTexture };
    Instance.Panels.push_back(ToggleWireframePanelWithTextTexture);

    // Toggle complex lighting
    SnazzCraft::TextTexture* ToggleComplexLightingTextTexture = new SnazzCraft::TextTexture{ "Toggle Complex Lighting", 0xFF, 0xFF, 0xFF };

    SnazzCraft::PanelWithTexture* ToggleComplexLightingPanelWithTexture = new SnazzCraft::PanelWithTexture(10.0f, ToggleWireframePanelWithTexture->GetY() + ToggleWireframePanelWithTexture->GetHeight() + 10.0f, ToggleComplexLightingTextTexture->Width, ToggleComplexLightingTextTexture->Height, 3.0f, "textures/gui/button.png");
    ToggleComplexLightingPanelWithTexture->SetCallback(SnazzCraft::ToggleComplexLightingCallback);
    Instance.Panels.push_back(ToggleComplexLightingPanelWithTexture);

    SnazzCraft::PanelWithTextTexture* ToggleComplexLightingPanelWithTextTexture = new SnazzCraft::PanelWithTextTexture{ 10.0f, ToggleWireframePanelWithTexture->GetY() + ToggleWireframePanelWithTexture->GetHeight() + 10.0f, 3.0f, ToggleComplexLightingTextTexture };
    Instance.Panels.push_back(ToggleComplexLightingPanelWithTextTexture);

    Instance.SetInputHandler(SnazzCraft::Window);
    Instance.SetInputHandlerCallback(InputHandlerCallback);
}


