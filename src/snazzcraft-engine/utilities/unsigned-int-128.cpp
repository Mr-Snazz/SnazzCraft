#include "snazzcraft-engine/utilities/unsigned-int-128.hpp"

SnazzCraft::UnsignedInt128::UnsignedInt128(uint64_t ILow)
    : Low(ILow), High(0u)
{

}

SnazzCraft::UnsignedInt128::UnsignedInt128(uint64_t ILow, uint64_t IHigh)
    : Low(ILow), High(IHigh)
{

}

