#pragma once

#include <stdint.h>

namespace SnazzCraft::TickSystem
{
    class Notification
    {
    public:
        enum class NotificationID : uint8_t
        {
            VoxelEditNextToWater
        };

        const NotificationID ID;

        Notification(NotificationID IID);

        ~Notification() = default;

        
    };
}