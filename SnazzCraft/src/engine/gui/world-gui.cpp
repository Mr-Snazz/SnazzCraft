#include "snazzcraft-engine/gui/world-gui.hpp"

SnazzCraft::InWorldGUI* SnazzCraft::WorldGUI = nullptr;

void SwapMenu(SnazzCraft::Event* Event);

SnazzCraft::InWorldGUI::InWorldGUI(unsigned int WindowWidth, unsigned int WindowHeight, GLFWwindow* Window) : SnazzCraft::GUI(WindowWidth, WindowHeight)
{
    this->SetInputHandler(new SnazzCraft::InputHandler(Window));

    // Hotbar
    this->WidgetCollections.push_back(new SnazzCraft::WidgetCollection()); 
    this->ActiveWidgetCollectionIndex = 1;

    this->WidgetCollections[1]->Panels.push_back(SNAZZCRAFT_PANEL_LONG(10, 10, new SnazzCraft::Texture("textures/gui/button.png"))); 
    this->WidgetCollections[1]->Buttons.push_back(SNAZZCRAFT_BUTTON_LONG(10, 10));
    this->WidgetCollections[1]->Buttons[0]->SwapPanels(new SnazzCraft::Texture());
    this->WidgetCollections[1]->Buttons[0]->ActivePanel->Texture->CreateTextureFromString("enter inventory", 255, 255, 255);
    this->WidgetCollections[1]->Buttons[0]->Callback = &SwapMenu;

    this->WidgetCollections[1]->Panels.push_back(SNAZZCRAFT_PANEL_LONG(10, 70, new SnazzCraft::Texture("textures/gui/button.png"))); 
    this->WidgetCollections[1]->Buttons.push_back(SNAZZCRAFT_BUTTON_LONG(10, 70));
    this->WidgetCollections[1]->Buttons[1]->SwapPanels(new SnazzCraft::Texture());
    this->WidgetCollections[1]->Buttons[1]->ActivePanel->Texture->CreateTextureFromString("activiate wireframe", 255, 255, 255);
    this->WidgetCollections[1]->Buttons[1]->Callback = &ToggleWireframeCallback;

    // Inventory
    this->WidgetCollections.push_back(new SnazzCraft::WidgetCollection()); 

    this->WidgetCollections[2]->Panels.push_back(SNAZZCRAFT_PANEL_LONG(10, 10, new SnazzCraft::Texture("textures/gui/button.png"))); 
    this->WidgetCollections[2]->Buttons.push_back(SNAZZCRAFT_BUTTON_LONG(10, 10));
    this->WidgetCollections[2]->Buttons[0]->SwapPanels(new SnazzCraft::Texture());
    this->WidgetCollections[2]->Buttons[0]->ActivePanel->Texture->CreateTextureFromString("exit inventory", 255, 255, 255);
    this->WidgetCollections[2]->Buttons[0]->Callback = &SwapMenu;
}

void SwapMenu(SnazzCraft::Event* Event)
{
    SnazzCraft::InWorldGUI* GUI = static_cast<SnazzCraft::InWorldGUI*>(Event->EventData->AccessDataType(SNAZZCRAFT_DATA_TYPE_GUI_ADDRESS));
    if (GUI == nullptr) return;

    GUI->SwapMenus();
}
