#include "snazzcraft-engine/core/core.hpp"

inline void MouseButtonCallback(GLFWwindow* Window, int Button, int Action, int Mods)
{
    double MouseX, MouseY;
    glfwGetCursorPos(Window, &MouseX, &MouseY);

    unsigned char EventType;
    if (Button == GLFW_MOUSE_BUTTON_LEFT) {
        EventType = (Action == GLFW_PRESS) ? SNAZZCRAFT_EVENT_MOUSE_CLICK_LEFT_PRESS : SNAZZCRAFT_EVENT_MOUSE_CLICK_LEFT_RELEASE;
    } else if (Button == GLFW_MOUSE_BUTTON_RIGHT) {
        EventType = (Action == GLFW_PRESS) ? SNAZZCRAFT_EVENT_MOUSE_CLICK_RIGHT_PRESS : SNAZZCRAFT_EVENT_MOUSE_CLICK_RIGHT_RELEASE;
    } else {
        return;
    }

    SnazzCraft::Event* NewEvent = new SnazzCraft::Event(EventType);
    NewEvent->EventData->Items.push_back(static_cast<void*>(new glm::dvec2(MouseX, MouseY)));
    NewEvent->EventData->Types.push_back(SNAZZCRAFT_DATA_TYPE_DVEC2);

    switch (SnazzCraft::UserMode) {
        case SNAZZCRAFT_USER_MODE_WORLD:
        {
            SnazzCraft::WorldGUI& Instance = SnazzCraft::WorldGUI::GetInstance();

            Instance.AddEvent(NewEvent);

            break;
        }

        case SNAZZCRAFT_USER_MODE_MAIN_MENU:
        {
            SnazzCraft::MainMenuGUI& Instance = SnazzCraft::MainMenuGUI::GetInstance();

            Instance.AddEvent(NewEvent);

            break;
        }
            
    }
}