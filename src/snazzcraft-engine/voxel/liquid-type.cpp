#include "snazzcraft-engine/voxel/liquid-type.hpp"

const SnazzCraft::LiquidType& SnazzCraft::LiquidType::GetFluidType(LiquidTypeID ID)
{
    static const SnazzCraft::LiquidType Water(5u,  8u);
    static const SnazzCraft::LiquidType Lava (10u, 8u);

    switch (ID)
    {
    case SnazzCraft::LiquidType::LiquidTypeID::Water:
        return Water;

    case SnazzCraft::LiquidType::LiquidTypeID::Lava:
        return Lava;
    
    default:
        return Water;
    }
}

SnazzCraft::LiquidType::LiquidType(uint8_t IFlow, uint8_t IMaximumReach)
    : Flow(IFlow), MaximumReach(IMaximumReach)
{

}