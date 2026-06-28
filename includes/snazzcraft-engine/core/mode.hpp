#pragma once

#include <stdint.h>

namespace SnazzCraft
{
    enum class UserMode : uint8_t
    {
        Overworld,
        Nether,
        End,
        MainMenu
    };

    // Modes are designed to be thread safe to read and write to concurrently

    SnazzCraft::UserMode GetUserMode();

    void SetUserMode(SnazzCraft::UserMode UserMode);

    bool ShouldCloseApplication();

    void SetShouldCloseApplication(bool Value);

    bool WireframeModeActive();

    void SetWireframeModeActive(bool Value);
} // SnazzCraft

