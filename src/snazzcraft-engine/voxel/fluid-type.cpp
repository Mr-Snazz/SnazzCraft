#include "snazzcraft-engine/voxel/fluid-type.hpp"

const SnazzCraft::FluidType& SnazzCraft::FluidType::GetFluidType(FluidTypeID ID)
{
    static const SnazzCraft::FluidType Water(5u, 8u);

    switch (ID)
    {
    case SnazzCraft::FluidType::FluidTypeID::Water:
        return Water;
    
    default:
        return Water;
    }
}

SnazzCraft::FluidType::FluidType(uint8_t IFlow, uint8_t IMaximumReach)
    : Flow(IFlow), MaximumReach(IMaximumReach)
{

}