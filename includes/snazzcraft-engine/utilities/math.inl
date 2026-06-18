#include "snazzcraft-engine/utilities/math.hpp"

inline void SnazzCraft::MoveVector3D(glm::vec3& Vector, const glm::vec3& Rotation, float Distance)
{
    glm::vec3 Front = SnazzCraft::CalculateFrontVector(Rotation, true);
    Vector += Front * Distance;
}

inline void SnazzCraft::MoveVector3DWithFront(glm::vec3& Vector, const glm::vec3& Front, float Distance)
{
    Vector += Front * Distance;
}

template <typename T>
requires std::signed_integral<T> || std::unsigned_integral<T>
constexpr T SnazzCraft::Index2D(T X, T Y, T Width)
{
    return Y * Width + X;
}

template <typename T>
requires std::signed_integral<T> || std::unsigned_integral<T>
constexpr T SnazzCraft::Index3D(T X, T Y, T Z, T Width, T Height)
{
    return X + (Y * Width) + (Z * Width * Height);
}

template <typename T>
requires std::signed_integral<T> && (sizeof(T) <= 4u)
constexpr uint64_t SnazzCraft::IntegerHash(T X, T Y)
{
    using Unsigned = std::make_unsigned_t<T>;

    uint64_t Low  = static_cast<Unsigned>(X);
    uint64_t High = static_cast<Unsigned>(Y);

    return (High << 32u) | Low;
}