#include "snazzcraft-engine/world/light-node.hpp"

SnazzCraft::World::LightNode::LightNode(int8_t ILightValue, int32_t IX, int32_t IY, int32_t IZ)
    : X(IX), Y(IY), Z(IZ), LightValue(ILightValue)
{

}

SnazzCraft::World::LightNode::LightNode(int8_t ILightValue, int32_t IPosition[3])
    : X(IPosition[0]), Y(IPosition[1]), Z(IPosition[2]), LightValue(ILightValue)
{
    
}