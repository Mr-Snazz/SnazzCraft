#include "snazzcraft-engine/utilities/byte-handling.hpp"

inline void SnazzCraft::ChangeValueInByte(uint8_t& Byte, uint8_t Index, bool Value)
{
    if (Index > 7u) return;
    if (Value) {
        Byte |= (1u << Index);
    } else {
        Byte &= ~(1u << Index);
    }
}

inline bool SnazzCraft::AccessBitValue(const uint8_t& Byte, uint8_t Index)
{
    if (Index > 7u) return false;
    return (Byte >> Index) & true;
}