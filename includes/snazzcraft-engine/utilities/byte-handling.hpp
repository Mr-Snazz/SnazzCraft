#pragma once

#include <stdint.h>

namespace SnazzCraft
{
    inline void ChangeValueInByte(uint8_t& Byte, uint8_t Index, bool Value);

    inline bool AccessBitValue(const uint8_t& Byte, uint8_t Index);
};

#include "snazzcraft-engine/utilities/byte-handling.inl"
