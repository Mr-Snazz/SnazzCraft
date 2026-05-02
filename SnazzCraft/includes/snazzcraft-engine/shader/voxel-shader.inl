#include "snazzcraft-engine/shader/voxel-shader.hpp"

inline const SnazzCraft::VoxelShader& SnazzCraft::VoxelShader::GetInstance()
{
    static VoxelShader Instance("src/glsl-shaders/voxel/voxel-vertex-shader.glsl", "src/glsl-shaders/voxel/voxel-fragment-shader.glsl");
    return Instance;
}

