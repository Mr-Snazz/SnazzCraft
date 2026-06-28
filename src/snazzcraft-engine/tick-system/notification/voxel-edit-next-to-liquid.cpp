#include "snazzcraft-engine/tick-system/notification/voxel-edit-next-to-liquid.hpp"

SnazzCraft::TickSystem::VoxelEditNextToLiquid::VoxelEditNextToLiquid(int32_t IChunkX, int32_t IChunkZ, uint32_t IVoxelIndex)
    : Notification(SnazzCraft::TickSystem::Notification::NotificationID::VoxelEditNextToLiquid), ChunkX(IChunkX), ChunkZ(IChunkZ), VoxelIndex(IVoxelIndex)
{

}
