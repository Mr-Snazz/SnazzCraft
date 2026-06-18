#include "snazzcraft-engine/mesh/mesh.hpp"

SnazzCraft::Mesh* SnazzCraft::VoxelMesh = nullptr;

SnazzCraft::Mesh::Mesh(std::vector<SnazzCraft::VoxelVertice> Vertices, std::vector<uint32_t> Indices, bool Initiate)
    : VAO(0u), VBO(0u), EBO(0u), ShouldInitiate(!Initiate)
{
	this->Vertices = Vertices;
	this->Indices = Indices;

    if (Initiate) this->Initiate();
}

SnazzCraft::Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
}

void SnazzCraft::Mesh::Draw() const
{
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->Indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void SnazzCraft::Mesh::UpdateGPUData(bool BindVAO, bool UnbindPostUpdate)
{
    if (this->ShouldInitiate) { this->ShouldInitiate = false; this->Initiate(); return; }

    if (BindVAO) glBindVertexArray(this->VAO);

    // Update VBO with current vertex data
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->Vertices.size() * sizeof(SnazzCraft::VoxelVertice), this->Vertices.data(), GL_DYNAMIC_DRAW);

    // Update EBO with current index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->Indices.size() * sizeof(uint32_t), this->Indices.data(), GL_DYNAMIC_DRAW);

    if (!UnbindPostUpdate) return;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SnazzCraft::Mesh::Initiate()
{
    if (this->VAO != 0u) glDeleteVertexArrays(1, &this->VAO);
    if (this->VBO != 0u) glDeleteBuffers(1, &this->VBO);
    if (this->EBO != 0u) glDeleteBuffers(1, &this->EBO);

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);

    this->UpdateGPUData(false, false); 

    // Set vertex attribute pointers

    // layout (location = 0) vec3 position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SnazzCraft::VoxelVertice), (void*)0);
    glEnableVertexAttribArray(0);

    // layout (location = 1) vec3 normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SnazzCraft::VoxelVertice), (void*)offsetof(SnazzCraft::VoxelVertice, SnazzCraft::VoxelVertice::Normal));
    glEnableVertexAttribArray(1);

    // layout (location = 2) vec2 texCoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SnazzCraft::VoxelVertice), (void*)offsetof(SnazzCraft::VoxelVertice, SnazzCraft::VoxelVertice::TextureCoordinate));
    glEnableVertexAttribArray(2);

    // layout (location = 3) float brightness
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(SnazzCraft::VoxelVertice), (void*)offsetof(SnazzCraft::VoxelVertice, SnazzCraft::VoxelVertice::Brightness));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

SnazzCraft::Mesh* SnazzCraft::Mesh::LoadMeshFromObjectFile(const char* FilePath, bool InitiateMesh) // Vibe coded for now
{
    std::vector<glm::vec3> TempPositions;
    std::vector<glm::vec2> TempUVs;
    std::vector<glm::vec3> TempNormals;

    std::vector<SnazzCraft::VoxelVertice> OutVertices;
    std::vector<uint32_t> OutIndices;

    std::ifstream File(FilePath);
    if (!File.is_open()) 
    {
        std::cerr << "SnazzCraft::Mesh Error: Could not open file at " << FilePath << std::endl;
        return nullptr;
    }

    std::string Line;
    while (std::getline(File, Line)) 
    {
        std::stringstream SS(Line);
        std::string Prefix;
        SS >> Prefix;

        if (Prefix == "v") // Position
        {
            glm::vec3 Pos;
            SS >> Pos.x >> Pos.y >> Pos.z;
            TempPositions.push_back(Pos);
        }
        else if (Prefix == "vt") // Texture Coordinate
        {
            glm::vec2 UV;
            SS >> UV.x >> UV.y;
            TempUVs.push_back(UV);
        }
        else if (Prefix == "vn") // Normal
        {
            glm::vec3 Norm;
            SS >> Norm.x >> Norm.y >> Norm.z;
            TempNormals.push_back(Norm);
        }
        else if (Prefix == "f") // Face
        {
            // Process the 3 vertices of the triangle
            for (int i = 0; i < 3; i++) 
            {
                std::string Segment;
                SS >> Segment;

                // Standard OBJ format is v/vt/vn. 
                // We replace slashes with spaces to extract indices easily via stream.
                for (auto& c : Segment) if (c == '/') c = ' ';
                
                std::stringstream SegmentSS(Segment);
                uint32_t PIdx, UIdx, NIdx;
                
                if (SegmentSS >> PIdx >> UIdx >> NIdx)
                {
                    SnazzCraft::VoxelVertice Vertex;
                    
                    // Wavefront indices are 1-based, so we subtract 1 for zero-based vectors
                    Vertex.Position  = TempPositions[PIdx - 1];
                    Vertex.Normal    = glm::vec3(0.0f);
                    Vertex.TextureCoordinate = TempUVs[UIdx - 1];

                    OutVertices.push_back(Vertex);
                    OutIndices.push_back(static_cast<uint32_t>(OutIndices.size()));
                }
            }
        }
    }

    File.close();

    // Instantiate the mesh with the populated vectors
    return new SnazzCraft::Mesh(OutVertices, OutIndices, InitiateMesh);
}


