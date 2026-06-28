#pragma once

#include "snazzcraft-engine/tick-system/notification/notification.hpp"

namespace SnazzCraft::TickSystem
{
    void Initialize();

    void AddNotification(SnazzCraft::TickSystem::Notification* Notification);
}