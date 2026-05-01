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
    constexpr T Index2D(T X, T Y, T Width);

    template <typename T>
    requires std::signed_integral<T> || std::unsigned_integral<T>
    constexpr T Index3D(T X, T Y, T Z, T Width, T Height);

    template <typename T>
    requires std::signed_integral<T> && (sizeof(T) <= 4u)
    constexpr uint64_t IntegerHash(T X, T Y);
} // SnazzCraft

#include "snazzcraft-engine/utilities/math.inl"


