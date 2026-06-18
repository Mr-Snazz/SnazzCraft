#pragma once

#include <stdint.h>
#include <unordered_map>

#include "external/noise/noise.h"

namespace SnazzCraft
{
    class HeightMap
    {
    public:
        std::unordered_map<uint64_t, int32_t> HeightValues = std::unordered_map<uint64_t, int>();

        int Seed;
        double Frequency;
        double Persistence;
        double Lacunarity;
        int OctaveCount;
        noise::module::Perlin NoiseModule;
        
        HeightMap(uint32_t ISize, int32_t IHeightConstraintLow, int32_t IHeightConstraintHigh, int32_t ISeed, double IFrequency, double IPersistence, double ILacunarity, int IOctaveCount);

        ~HeightMap() = default;

        void GenerateValue(int32_t X, int32_t Y);

    private:
        uint32_t Size;
        int32_t HeightConstraintLow;
        int32_t HeightConstraintHigh;

    };
} // SnazzCraft
