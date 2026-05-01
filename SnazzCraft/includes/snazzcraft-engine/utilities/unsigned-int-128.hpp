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
            
            bool Carry;
            UnsignedInt128::Add(this->Low, Addend.Low, Result.Low, Carry);

            Result.High = this->High + Addend.High + (Carry ? 1u : 0u);
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
        

    public:


    private:
        static inline void Add(uint64_t A, uint64_t B, uint64_t& Result, bool& Carry)
        {
            Result = A + B;
            Carry = (Result < A);
        }

    };
}