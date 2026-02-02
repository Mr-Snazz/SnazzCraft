#include "atlas.hpp"

SnazzCraft::VoxelTextureApplier* SnazzCraft::EngineVoxelTextureApplier = new SnazzCraft::VoxelTextureApplier("textures/voxel/atlas-coordinates.txt");

SnazzCraft::VoxelTextureApplier::VoxelTextureApplier(const char* AtlasFilePath) 
{ 
    this->LoadAtlasCoordinates(AtlasFilePath);
}

SnazzCraft::VoxelTextureApplier::~VoxelTextureApplier() 
{ 

}

std::vector<SnazzCraft::Vertice3D> SnazzCraft::VoxelTextureApplier::GetTexturedVertices(const Voxel& Voxel)
{
    const glm::vec2 HalfPixel(HALF_PIXEL, HALF_PIXEL);   

    std::vector<SnazzCraft::Vertice3D> Vertices;

    glm::vec2 AtlasCoordinates = { 
        (float)this->TextureCoordinates[Voxel.ID][0],
        (float)this->TextureCoordinates[Voxel.ID][1]
    };

    AtlasCoordinates /= (float)ATLAS_SIZE; 

    for (const SnazzCraft::Vertice3D& V : SnazzCraft::VoxelMesh->Vertices) {
        Vertices.push_back(SnazzCraft::Vertice3D((V.Position * SnazzCraft::VoxelMesh->ScaleVector), V.TextureCoordinate + AtlasCoordinates));
    }

    switch (Voxel.ID)
    {
        case ID_VOXEL_DIRT_GRASS_MIX:
        {
            Vertices[16].TextureCoordinate = { 0.4f, 0.2f };
            Vertices[17].TextureCoordinate = { 0.4f, 0.0f };
            Vertices[18].TextureCoordinate = { 0.6f, 0.0f };
            Vertices[19].TextureCoordinate = { 0.6f, 0.2f };

            Vertices[20].TextureCoordinate = { 0.6f, 0.2f };
            Vertices[21].TextureCoordinate = { 0.6f, 0.0f };
            Vertices[22].TextureCoordinate = { 0.8f, 0.0f };
            Vertices[23].TextureCoordinate = { 0.8f, 0.2f };

            break;
        }

        default:
            break;
    }

    return Vertices;
}

bool SnazzCraft::VoxelTextureApplier::LoadAtlasCoordinates(const char* AtlasFilePath)
{
    std::ifstream File(AtlasFilePath);
    std::string Line;

    this->TextureCoordinatesCount = 0;
    while (std::getline(File, Line) && this->TextureCoordinatesCount < ATLAS_SIZE * ATLAS_SIZE) 
    {
        if (Line.empty()) continue;

        std::stringstream SS(Line);
        SS >> this->TextureCoordinates[this->TextureCoordinatesCount][0] >> this->TextureCoordinates[this->TextureCoordinatesCount][1];

        this->TextureCoordinatesCount++;
    }

    File.close();

    return true;
}
