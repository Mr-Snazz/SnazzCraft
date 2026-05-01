#pragma once

#include <stdint.h>
#include <concepts>

#include "external/glm/glm.hpp"

#include "unsigned-int-128.hpp"

namespace SnazzCraft
{
    glm::vec3 CalculateFrontVector(const glm::vec3& Rotation, bool Normalize);

    void MoveVector3D(glm::vec3& Vector, const glm::vec3& Rotation, float Distance);

    void MoveVector3DWithFront(glm::vec3& Vector, const glm::vec3& Front, float Distance);

    template <typename T>
    requires std::signed_integral<T> || std::unsigned_integral<T>
    constexpr T Index2D(T X, T Y, T Width)
    {
        return Y * Width + X;
    }

    template <typename T>
    requires std::signed_integral<T> || std::unsigned_integral<T>
    constexpr T Index3D(T X, T Y, T Z, T Width, T Height)
    {
        return X + (Y * Width) + (Z * Width * Height);
    }

    template <typename T>
    requires std::signed_integral<T> && (sizeof(T) <= 4u)
    constexpr uint64_t IntegerHash(T X, T Y)
    {
        uint64_t Low  = static_cast<uint64_t>(X);
        uint64_t High = static_cast<uint64_t>(Y);
        return uint64_t((High << 32) | Low);
    }
} // SnazzCraft




