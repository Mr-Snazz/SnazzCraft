#include "snazzcraft-engine/core/core-callback.hpp"

#include "external/glad.h"
#include "snazzcraft-engine/input-handler/event.hpp"

#include "snazzcraft-engine/input-handler/data-types.h"
#include "snazzcraft-engine/world/world.hpp"
#include "snazzcraft-engine/core/mode.hpp"

void SnazzCraft::CreateWorldAndChangeModeToWorld(SnazzCraft::Event* Event)
{
    if (SnazzCraft::CurrentWorld != nullptr) goto SwitchMode;

    SnazzCraft::CurrentWorld = SnazzCraft::World::CreateWorld("TEST WORLD", 4, 80085);

    SwitchMode:
    SnazzCraft::UserMode = SNAZZCRAFT_USER_MODE_WORLD;
}

void SnazzCraft::ChangeModeToWorldCallback(SnazzCraft::Event* Event)
{
    if (SnazzCraft::UserMode == SNAZZCRAFT_USER_MODE_WORLD) return;

    SnazzCraft::UserMode = SNAZZCRAFT_USER_MODE_WORLD;
}

void SnazzCraft::ChangeModeToMainMenuCallback(SnazzCraft::Event* Event)
{
    if (SnazzCraft::UserMode == SNAZZCRAFT_USER_MODE_MAIN_MENU) return;

    SnazzCraft::UserMode = SNAZZCRAFT_USER_MODE_MAIN_MENU;
}

void SnazzCraft::ToggleWireframeCallback(SnazzCraft::Event* Event)
{
    SnazzCraft::WireframeModeActive = !SnazzCraft::WireframeModeActive;
}