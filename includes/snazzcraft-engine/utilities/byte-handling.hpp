#pragma once

#include <stdint.h>

namespace SnazzCraft
{
    inline void ChangeValueInByte(uint8_t& Byte, const uint8_t& Index, const bool& Value);

    inline bool AccessBitValue(const uint8_t& Byte, const uint8_t& Index);
};

#include "snazzcraft-engine/utilities/byte-handling.inl"
