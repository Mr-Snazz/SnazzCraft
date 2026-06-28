#include <atomic>

#include "snazzcraft-engine/core/mode.hpp"

std::atomic<SnazzCraft::UserMode> CurrentUserModeState       {};
std::atomic<bool>                 ShouldCloseApplicationState{};
std::atomic<bool>                 WireframeModeState         {};

SnazzCraft::UserMode SnazzCraft::GetUserMode()
{
    return CurrentUserModeState.load();
}

void SnazzCraft::SetUserMode(SnazzCraft::UserMode UserMode)
{
    CurrentUserModeState.store(UserMode);
}

bool SnazzCraft::ShouldCloseApplication()
{
    return ShouldCloseApplicationState.load();
}

void SnazzCraft::SetShouldCloseApplication(bool Value)
{
    ShouldCloseApplicationState.store(Value);
}

bool SnazzCraft::WireframeModeActive()
{
    return WireframeModeState.load();
}

void SnazzCraft::SetWireframeModeActive(bool Value)
{
    WireframeModeState.store(Value);
}

