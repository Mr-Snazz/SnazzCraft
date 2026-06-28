#include "snazzcraft-engine/voxel/voxel.hpp"

SnazzCraft::Voxel::Voxel()
    : ID(SnazzCraft::VoxelType::VoxelTypeID::Air)
{

}

SnazzCraft::Voxel::Voxel(SnazzCraft::VoxelType::VoxelTypeID IID)
    : ID(IID)
{

}


