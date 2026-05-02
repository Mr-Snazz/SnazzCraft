#pragma once

#include <stdint.h>

namespace SnazzCraft
{
    class UnsignedInt128
    {
    public:
        uint64_t Low, High;

        UnsignedInt128() = default;

        UnsignedInt128(uint64_t Low);

        UnsignedInt128(uint64_t ILow, uint64_t IHigh);

        UnsignedInt128 operator+(const UnsignedInt128& Addend) const
        {
            UnsignedInt128 Result;
            Result.Low = this->Low + Addend.Low;
            Result.High = this->High + Addend.High + ((Result.Low < this->Low) ? 1u : 0u);

            return Result;
        }

        UnsignedInt128 operator*(const UnsignedInt128& Factor) const
        {
            uint64_t ALow  = this->Low  & 0xFFFFFFFF;
            uint64_t AHigh = this->Low >> 32u;
            uint64_t BLow = Factor.Low & 0xFFFFFFFF;
            uint64_t BHigh = Factor.Low >> 32u;

            uint64_t NodeA = ALow * BLow;
            uint64_t NodeB = AHigh * BLow + (NodeA >> 32u);
            uint64_t NodeC = ALow * BHigh + (NodeB & 0xFFFFFFFF);

            uint64_t ResultLow = (NodeC << 32u) | (NodeA & 0xFFFFFFFF);
            uint64_t CarryLow = (NodeB >> 32u) + (NodeC >> 32u);

            uint64_t CrossTerms = (this->Low * Factor.High) + (this->High * Factor.Low);
            uint64_t ResultHigh = CrossTerms + CarryLow;

            return UnsignedInt128(ResultLow, ResultHigh);
        }

    private:
        


    };
}