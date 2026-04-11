#pragma once

namespace SnazzCraft
{
    class Event;

    void CreateWorldAndChangeModeToWorld(SnazzCraft::Event* Event);

    void ChangeModeToWorldCallback(SnazzCraft::Event* Event);

    void ChangeModeToMainMenuCallback(SnazzCraft::Event* Event);

    void CloseWindowCallback(SnazzCraft::Event* Event);

    void ToggleWireframeCallback(SnazzCraft::Event* Event);
} // SnazzCraft


