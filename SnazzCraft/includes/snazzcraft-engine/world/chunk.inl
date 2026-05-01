#include "snazzcraft-engine/world/chunk.hpp"

inline void SnazzCraft::Chunk::UpdateMesh()
{
    delete this->ChunkMesh;
    if (this->Vertices.empty() || this->Indices.empty()) { this->ChunkMesh = nullptr; return; }

    this->ChunkMesh = new SnazzCraft::Mesh(this->Vertices, this->Indices);
}

inline glm::vec3 SnazzCraft::Chunk::LocalVoxelPositionToWorldPosition(uint32_t X, uint32_t Y, uint32_t Z) const
{
    return glm::vec3((float)X, (float)Y, (float)Z) * glm::vec3((float)SnazzCraft::Voxel::Size, (float)SnazzCraft::Voxel::Size, (float)SnazzCraft::Voxel::Size) + this->ChunkWorldOffset;
}

inline void SnazzCraft::Chunk::WorldSpaceToVoxelSpace(const glm::vec3& WorldPosition, int32_t VoxelPosition[3]) const
{
    glm::vec3 LocalPosition = WorldPosition - this->ChunkWorldOffset;

    VoxelPosition[0] = static_cast<int>(LocalPosition.x / SnazzCraft::Voxel::Size);
    VoxelPosition[1] = static_cast<int>(LocalPosition.y / SnazzCraft::Voxel::Size);
    VoxelPosition[2] = static_cast<int>(LocalPosition.z / SnazzCraft::Voxel::Size);
}


inline void SnazzCraft::Chunk::GetChunkPosition(const glm::vec3& Position, int32_t OutChunkPosition[2]) // Voxel space coordinates
{
    OutChunkPosition[0] = static_cast<int>(Position.x) / SnazzCraft::Chunk::Width;
    OutChunkPosition[1] = static_cast<int>(Position.z) / SnazzCraft::Chunk::Depth;
}

inline void SnazzCraft::Chunk::GetChunkPosition(int32_t X, int32_t Z, int32_t OutChunkPosition[2]) // Voxel space coordinates
{
    OutChunkPosition[0] = X / SnazzCraft::Chunk::Width;
    OutChunkPosition[1] = Z / SnazzCraft::Chunk::Depth;
}

inline void SnazzCraft::Chunk::GetLocalVoxelPosition(int32_t X, int32_t Y, int32_t Z, int32_t OutLocalVoxelPosition[3]) // Voxel space coordinates
{
    OutLocalVoxelPosition[0] = X % SnazzCraft::Chunk::Width;
    OutLocalVoxelPosition[1] = Y % SnazzCraft::Chunk::Height;
    OutLocalVoxelPosition[2] = Z % SnazzCraft::Chunk::Depth;
}

constexpr uint32_t SnazzCraft::Chunk::LocalVoxelIndex(int32_t X, int32_t Y, int32_t Z)
{
    return SnazzCraft::Index3D<uint32_t>(X, Y, Z, static_cast<uint32_t>(SnazzCraft::Chunk::Width), static_cast<uint32_t>(SnazzCraft::Chunk::Height));
}

constexpr uint32_t SnazzCraft::Chunk::LocalVoxelIndex(uint32_t X, uint32_t Y, uint32_t Z)
{
    return SnazzCraft::Index3D<uint32_t>(X, Y, Z, static_cast<uint32_t>(SnazzCraft::Chunk::Width), static_cast<uint32_t>(SnazzCraft::Chunk::Height));
}

inline uint32_t SnazzCraft::Chunk::LocalVoxelIndex(const SnazzCraft::Voxel& Voxel)
{
    return SnazzCraft::Index3D<uint32_t>(Voxel.X, Voxel.Y, Voxel.Z, static_cast<uint32_t>(SnazzCraft::Chunk::Width), static_cast<uint32_t>(SnazzCraft::Chunk::Height));
}

constexpr bool SnazzCraft::Chunk::WithinChunkBounds(int32_t X, int32_t Y, int32_t Z)
{
    return X >= 0 && Y >= 0 && Z >= 0 && X < SnazzCraft::Chunk::Width && Y < SnazzCraft::Chunk::Height && Z < SnazzCraft::Chunk::Depth;
}

constexpr bool SnazzCraft::Chunk::WithinChunkBounds(uint32_t X, uint32_t Y, uint32_t Z)
{
    return X < SnazzCraft::Chunk::Width && Y < SnazzCraft::Chunk::Height && Z < SnazzCraft::Chunk::Depth;
}







