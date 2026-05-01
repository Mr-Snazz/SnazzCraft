#pragma once

#include <stdint.h>
#include <unordered_map>

namespace SnazzCraft
{
    class PerlinNoiseModule;

    struct HeightMap
    {
    public:
        std::unordered_map<uint64_t, int32_t> HeightValues = std::unordered_map<uint64_t, int>();
        SnazzCraft::PerlinNoiseModule* NoiseModule = nullptr;
        
        HeightMap(uint32_t Size, int32_t HeightConstraintLow, int32_t HeightConstraintHigh, int32_t Seed, double Frequency, double Persistence, double Lacunarity, int OctaveCount);
        ~HeightMap();

        void GenerateValue(int32_t X, int32_t Y);

    private:
        uint32_t Size;
        int32_t HeightConstraintLow;
        int32_t HeightConstraintHigh;

    };
} // SnazzCraft
