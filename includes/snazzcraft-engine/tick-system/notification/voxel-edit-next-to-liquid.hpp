#pragma once

#include "snazzcraft-engine/tick-system/notification/notification.hpp"

namespace SnazzCraft::TickSystem
{
    class VoxelEditNextToLiquid : public SnazzCraft::TickSystem::Notification
    {
    public:
        union 
        {
            struct 
            {
                const int32_t ChunkX, ChunkZ;
            };
            const int32_t ChunkPosition[2];
        };
            
        const uint32_t VoxelIndex;

        VoxelEditNextToLiquid(int32_t IChunkX, int32_t IChunkZ, uint32_t IVoxelIndex);

    private:


    };
}


