#pragma once

namespace SnazzCraft
{
    class Event;

    inline void CreateWorldAndChangeModeToWorld(SnazzCraft::Event* Event);

    inline void ChangeModeToWorldCallback(SnazzCraft::Event* Event);

    inline void ChangeModeToMainMenuCallback(SnazzCraft::Event* Event);

    inline void CloseWindowCallback(SnazzCraft::Event* Event);

    inline void ToggleWireframeCallback(SnazzCraft::Event* Event);

    inline void ToggleComplexLightingCallback(SnazzCraft::Event* Event);
} // SnazzCraft

#include "snazzcraft-engine/core/core-callback.inl"
