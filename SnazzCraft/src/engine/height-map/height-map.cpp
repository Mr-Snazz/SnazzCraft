#include "snazzcraft-engine/height-map/height-map.hpp"

SnazzCraft::HeightMap::HeightMap(unsigned int Size, int HeightConstraintLow, int HeightConstraintHigh, int Seed, double Frequency, double Persistence, double Lacunarity, int OctaveCount)
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

void SnazzCraft::HeightMap::GenerateValue(unsigned int X, unsigned int Y)
{
    auto HeightValueIterator = this->HeightValues.find(INDEX_2D(X, Y, this->Size));
    if (HeightValueIterator != this->HeightValues.end()) return;

    const double NX = (double)(X) * 0.005;
    const double NY = (double)(Y) * 0.005;

    double HeightValue = this->NoiseModule->NoiseModule.GetValue(NX, NY, 0.0);
    HeightValue *= 16.0;

    HeightValue = HeightValue < this->HeightConstraintLow  ? this->HeightConstraintLow  : HeightValue;
    HeightValue = HeightValue > this->HeightConstraintHigh ? this->HeightConstraintHigh : HeightValue;

    this->HeightValues[INDEX_2D(X, Y, this->Size)] = (int)HeightValue;
}
