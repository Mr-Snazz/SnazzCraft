#include "snazzcraft-engine/height-map/height-map.hpp"
#include "snazzcraft-engine/height-map/noise-module.hpp"
#include "snazzcraft-engine/utilities/math.hpp"

SnazzCraft::HeightMap::HeightMap(uint32_t Size, int32_t HeightConstraintLow, int32_t HeightConstraintHigh, int32_t Seed, double Frequency, double Persistence, double Lacunarity, int OctaveCount)
{
    this->NoiseModule = new SnazzCraft::PerlinNoiseModule(Seed, Frequency, Persistence, Lacunarity, OctaveCount);

    this->Size = Size;
    this->HeightConstraintLow = HeightConstraintLow;
    this->HeightConstraintHigh = HeightConstraintHigh;
}

SnazzCraft::HeightMap::~HeightMap()
{
    delete this->NoiseModule;
}

void SnazzCraft::HeightMap::GenerateValue(uint32_t X, uint32_t Y)
{
    auto HeightValueIterator = this->HeightValues.find(SnazzCraft::Index2D(X, Y, this->Size));
    if (HeightValueIterator != this->HeightValues.end()) return;

    const double NX = (double)(X) * 0.005;
    const double NY = (double)(Y) * 0.005;

    double HeightValue = this->NoiseModule->NoiseModule.GetValue(NX, NY, 0.0);
    HeightValue *= 16.0;

    HeightValue = HeightValue < this->HeightConstraintLow  ? this->HeightConstraintLow  : HeightValue;
    HeightValue = HeightValue > this->HeightConstraintHigh ? this->HeightConstraintHigh : HeightValue;

    this->HeightValues[SnazzCraft::Index2D(X, Y, this->Size)] = (int32_t)HeightValue;
}
