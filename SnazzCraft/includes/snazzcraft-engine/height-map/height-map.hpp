#pragma once

#include <unordered_map>

namespace SnazzCraft
{
    class PerlinNoiseModule;

    struct HeightMap
    {
    public:
        std::unordered_map<unsigned int, int> HeightValues = std::unordered_map<unsigned int, int>();
        SnazzCraft::PerlinNoiseModule* NoiseModule = nullptr;
        

        HeightMap(unsigned int Size, int HeightConstraintLow, int HeightConstraintHigh, int Seed, double Frequency, double Persistence, double Lacunarity, int OctaveCount);
        ~HeightMap();

        void GenerateValue(unsigned int X, unsigned int Y);

    private:
        unsigned int Size;
        int HeightConstraintLow;
        int HeightConstraintHigh;

    };
} // SnazzCraft
