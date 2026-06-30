#pragma once

namespace SnazzCraft::TickSystem
{
    constexpr uint8_t TicksPerSecond = 20u;
    constexpr uint8_t RedstoneTicksPerSecond = TicksPerSecond / 2u;

    void Initialize();
}