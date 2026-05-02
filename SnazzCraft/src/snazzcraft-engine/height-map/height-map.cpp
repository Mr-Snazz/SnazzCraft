#include "snazzcraft-engine/height-map/height-map.hpp"
#include "snazzcraft-engine/utilities/math.hpp"

SnazzCraft::HeightMap::HeightMap(uint32_t ISize, int32_t IHeightConstraintLow, int32_t IHeightConstraintHigh, int32_t ISeed, double IFrequency, double IPersistence, double ILacunarity, int IOctaveCount)
    : Seed(ISeed), Frequency(IFrequency), Persistence(IPersistence), Lacunarity(ILacunarity), OctaveCount(IOctaveCount), Size(ISize), HeightConstraintLow(IHeightConstraintLow), HeightConstraintHigh(IHeightConstraintHigh)
{
    this->NoiseModule.SetSeed(this->Seed);
    this->NoiseModule.SetFrequency(this->Frequency);
    this->NoiseModule.SetPersistence(this->Persistence);
    this->NoiseModule.SetLacunarity(this->Lacunarity);
    this->NoiseModule.SetOctaveCount(this->OctaveCount);
}

void SnazzCraft::HeightMap::GenerateValue(int32_t X, int32_t Y)
{
    constexpr double Scale = 0.005;

    uint64_t Hash = SnazzCraft::IntegerHash(X, Y);
    auto HeightValueIterator = this->HeightValues.find(Hash);
    if (HeightValueIterator != this->HeightValues.end()) return;

    const double NX = static_cast<double>(X) * Scale;
    const double NY = static_cast<double>(Y) * Scale;

    double HeightValue = this->NoiseModule.GetValue(NX, NY, 0.0) * 16.0;

    HeightValue = HeightValue < this->HeightConstraintLow  ? this->HeightConstraintLow  : HeightValue;
    HeightValue = HeightValue > this->HeightConstraintHigh ? this->HeightConstraintHigh : HeightValue;

    this->HeightValues[Hash] = static_cast<int32_t>(HeightValue);
}
