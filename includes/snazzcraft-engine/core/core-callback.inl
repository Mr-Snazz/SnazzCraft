#include "snazzcraft-engine/core/core-callback.hpp"

#include "external/glad.h"
#include "snazzcraft-engine/input-handler/event.hpp"

#include "snazzcraft-engine/input-handler/data-types.h"
#include "snazzcraft-engine/world/world.hpp"
#include "snazzcraft-engine/core/mode.hpp"
#include "snazzcraft-engine/shader/voxel-shader.hpp"

inline void SnazzCraft::CreateWorldAndChangeModeToWorld(SnazzCraft::Event* Event)
{
    if (!SnazzCraft::Overworld) SnazzCraft::Overworld = SnazzCraft::World::CreateWorld("TEST WORLD", 30, 80085);

    SnazzCraft::SetUserMode(SnazzCraft::UserMode::Overworld);
}

inline void SnazzCraft::ChangeModeToWorldCallback(SnazzCraft::Event* Event)
{
    SnazzCraft::SetUserMode(SnazzCraft::UserMode::Overworld);
}

inline void SnazzCraft::ChangeModeToMainMenuCallback(SnazzCraft::Event* Event)
{
    SnazzCraft::SetUserMode(SnazzCraft::UserMode::MainMenu);
}

inline void SnazzCraft::ToggleWireframeCallback(SnazzCraft::Event* Event)
{
    SnazzCraft::SetWireframeModeActive(!SnazzCraft::WireframeModeActive());
}

