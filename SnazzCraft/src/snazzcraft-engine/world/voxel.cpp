#include "snazzcraft-engine/world/voxel.hpp"
#include "snazzcraft-engine/world/voxel-type.hpp"
#include "snazzcraft-engine/world/voxel-ids.h"

SnazzCraft::Voxel::Voxel()
    : X(0x00), Y(0x00), Z(0x00), ID(ID_VOXEL_AIR)
{

}

SnazzCraft::Voxel::Voxel(uint8_t IX, uint8_t IY, uint8_t IZ, uint8_t IID)
    : X(IX), Y(IY), Z(IZ), ID(IID)
{

}

const SnazzCraft::VoxelType& SnazzCraft::Voxel::GetVoxelType() const
{
    return SnazzCraft::VoxelType::GetVoxelType(this->ID);
}
