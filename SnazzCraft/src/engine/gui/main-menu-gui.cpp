#include "snazzcraft-engine/gui/main-menu-gui.hpp"
#include "snazzcraft-engine/input-handler/input-handler.hpp"
#include "snazzcraft-engine/gui/widget-collection.hpp"
#include "snazzcraft-engine/gui/button.hpp"
#include "snazzcraft-engine/gui/panel.hpp"
#include "snazzcraft-engine/texture/texture.hpp"

SnazzCraft::MainMenuGUI* SnazzCraft::MenuGUI = nullptr;

SnazzCraft::MainMenuGUI::MainMenuGUI(unsigned int WindowWidth, unsigned int WindowHeight, GLFWwindow* Window) : SnazzCraft::GUI(WindowWidth, WindowHeight)
{
    this->SetInputHandler(new SnazzCraft::InputHandler(Window));

    this->WidgetCollections.push_back(new SnazzCraft::WidgetCollection());
    this->ActiveWidgetCollectionIndex = 1;

    this->WidgetCollections[1]->Panels.push_back(new SnazzCraft::Panel(0, 0, WindowWidth, WindowHeight, new SnazzCraft::Texture("textures/voxel/individuals/dirt.png"))); // Backround

    this->WidgetCollections[1]->Panels.push_back(SnazzCraft::Panel::NewPanelLong((int)(WindowWidth / 2.0f) - (int)(SNAZZCRAFT_PANEL_LONG_WIDTH / 2.0f), (int)(WindowHeight / 4), new SnazzCraft::Texture("textures/gui/button.png")));
    this->WidgetCollections[1]->Buttons.push_back(SnazzCraft::Button::NewButtonLong((int)(WindowWidth / 2.0f) - (int)(SNAZZCRAFT_PANEL_LONG_WIDTH / 2.0f), (int)(WindowHeight / 4)));
    this->WidgetCollections[1]->Buttons[0]->SwapPanels(new SnazzCraft::Texture());
    this->WidgetCollections[1]->Buttons[0]->ActivePanel->Texture->CreateTextureFromString("load world", 255, 255, 255);
    this->WidgetCollections[1]->Buttons[0]->Callback = &SetWorldModeButtonCallback;
}
