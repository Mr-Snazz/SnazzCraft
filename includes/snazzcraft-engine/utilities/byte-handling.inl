#include "snazzcraft-engine/utilities/byte-handling.hpp"

inline void SnazzCraft::ChangeValueInByte(uint8_t& Byte, const uint8_t& Index, const bool& Value)
{
if (Index > 7) return;
if (Value) {
    Byte |= (1 << Index);
} else {
    Byte &= ~(1 << Index);
}
}

inline bool SnazzCraft::AccessBitValue(const uint8_t& Byte, const uint8_t& Index)
{
if (Index > 7) return false;
return (Byte >> Index) & true;
}