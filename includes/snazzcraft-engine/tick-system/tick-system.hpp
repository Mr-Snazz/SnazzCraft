#pragma once

#include "snazzcraft-engine/tick-system/notification/notification.hpp"

namespace SnazzCraft::TickSystem
{
    constexpr uint8_t TicksPerSecond = 20u;
    constexpr uint8_t RedstoneTicksPerSecond = TicksPerSecond / 2u;

    void Initialize();

    void AddNotification(SnazzCraft::TickSystem::Notification* Notification);
}