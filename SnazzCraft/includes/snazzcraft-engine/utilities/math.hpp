#pragma once

#include <concepts>

#include "glm/glm.hpp"

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
} // SnazzCraft




