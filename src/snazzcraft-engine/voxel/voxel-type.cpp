#include "snazzcraft-engine/voxel/voxel-type.hpp"
#include "snazzcraft-engine/voxel/voxel.hpp"

const SnazzCraft::VoxelType& SnazzCraft::VoxelType::GetVoxelType(VoxelTypeID VoxelID)
{
    //                                                 LightProducingLevel, LightPropogationDecrease, CullableSides, CollidableToEntities, CollidableToVoxels. CullableAgainst
    // NOTE: The two most significant bits of CullableSides are never set, and will never be used even if they are 
    static const SnazzCraft::VoxelType Leaves         (0,  2,                                0b00000000, true,  true,  true );
    static const SnazzCraft::VoxelType VoxelCollidable(0,  0,                                0b00000000, false, true,  true );
    static const SnazzCraft::VoxelType Torch          (18, 0,                                0b00000000, false, true,  true );
    static const SnazzCraft::VoxelType Water          (0,  2,                                0b00111111, true,  false, true );
    static const SnazzCraft::VoxelType Lava           (9,  4,                                0b00111111, true,  false, true );
    static const SnazzCraft::VoxelType FullSolid      (0,  SnazzCraft::Voxel::MaxLightValue, 0b00111111, true,  true,  true );
    static const SnazzCraft::VoxelType Phantom        (0,  0,                                0b00111111, false, false, false);

    switch (VoxelID)
    {
        case SnazzCraft::VoxelType::VoxelTypeID::OakLeaves:
            return Leaves;

        case SnazzCraft::VoxelType::VoxelTypeID::Water:
            return Water;

        case SnazzCraft::VoxelType::VoxelTypeID::Lava:
            return Lava;

        case SnazzCraft::VoxelType::VoxelTypeID::AboveGrass:
            return VoxelCollidable;

        case SnazzCraft::VoxelType::VoxelTypeID::Torch:
            return Torch;

        case SnazzCraft::VoxelType::VoxelTypeID::Air:
            return Phantom;

        default:
            return FullSolid;
    }
}

SnazzCraft::VoxelType::VoxelType(int8_t ILightProducingLevel, int8_t ILightPropogationDecrease, uint8_t ICullableSides, bool ICollidableToEntities, bool ICollidableToVoxels, bool ICullableAgainst)
    : LightProducingLevel(ILightProducingLevel), LightPropogationDecrease(ILightPropogationDecrease), 
      CullableSides      (ICullableSides),       CollidableToEntities    (ICollidableToEntities), 
      CollidableToVoxels (ICollidableToVoxels),  CullableAgainst         (ICullableAgainst)
{

}


