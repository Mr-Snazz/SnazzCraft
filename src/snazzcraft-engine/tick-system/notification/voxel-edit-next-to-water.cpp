#include "snazzcraft-engine/tick-system/notification/voxel-edit-next-to-water.hpp"

SnazzCraft::TickSystem::VoxelEditNextToWater::VoxelEditNextToWater(int32_t IChunkX, int32_t IChunkZ, uint32_t IVoxelIndex)
    : Notification(SnazzCraft::TickSystem::Notification::NotificationID::VoxelEditNextToWater), ChunkX(IChunkX), ChunkZ(IChunkZ), VoxelIndex(IVoxelIndex)
{

}
