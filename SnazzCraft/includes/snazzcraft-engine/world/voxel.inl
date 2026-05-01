#include "snazzcraft-engine/world/voxel.hpp"
#include "snazzcraft-engine/utilities/byte-handling.hpp"

inline bool SnazzCraft::Voxel::HasSide(uint8_t SideIndex) const
{
    return SnazzCraft::AccessBitValue(this->Sides, SideIndex);
}

inline void SnazzCraft::Voxel::ChangeSideValue(uint8_t SideIndex, bool Value)
{
    SnazzCraft::ChangeValueInByte(this->Sides, SideIndex, Value);
}

inline void SnazzCraft::Voxel::SetAllSides()
{
    this->Sides = 0x3F;
}

inline void SnazzCraft::Voxel::ClearAllSides()
{
    this->Sides = 0x00;
}

inline uint8_t SnazzCraft::Voxel::GetSideCount() const
{
    uint8_t Count = 0;
    for (uint8_t I = 0; I < 6; I++) {
        if (this->HasSide(I)) Count++;
    }

    return Count;
}