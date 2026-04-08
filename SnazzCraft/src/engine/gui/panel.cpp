#include "snazzcraft-engine/gui/panel.hpp"
#include "snazzcraft-engine/utilities/math.hpp"
#include "snazzcraft-engine/texture/texture.hpp"

const float FullTextureCoordinates[12] = {
    0.0f, 1.0f, // Bottom Left
    0.0f, 0.0f, // Top Left
    1.0f, 0.0f, // Top Right
    1.0f, 0.0f, // Top Right
    0.0f, 1.0f, // Bottom Left
    1.0f, 1.0f  // Bottom Right
};

const unsigned char FullScreenIndexesX[3] = {
    4, 6, 10
};

const unsigned char FullScreenIndexesY[3] = {
    1, 9, 11
};

SnazzCraft::Panel::Panel(unsigned int X, unsigned int Y, unsigned int Width, unsigned int Height, SnazzCraft::Texture* Texture)
{
    this->Texture = Texture;

    this->Initiate(X, Y, Width, Height);
}

SnazzCraft::Panel::~Panel()
{
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
   
    delete this->Texture;
}

void SnazzCraft::Panel::Draw()
{
    if (this->Texture == nullptr) return;

    if (!this->Texture->BindTexture()) return;

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SnazzCraft::Panel::UpdateVertices(unsigned int X, unsigned int Y, unsigned int Width, unsigned int Height) 
{
    glBindVertexArray(this->VAO);

    float Vertices[24];
    // Copy position data
    Vertices[SnazzCraft::Index2D(0, 0, 4)] = static_cast<float>(X);          // Bottom Left X
    Vertices[SnazzCraft::Index2D(1, 0, 4)] = static_cast<float>(Y + Height); // Bottom Left Y
    Vertices[SnazzCraft::Index2D(0, 1, 4)] = static_cast<float>(X);          // Top Left X
    Vertices[SnazzCraft::Index2D(1, 1, 4)] = static_cast<float>(Y);          // Top Left Y
    Vertices[SnazzCraft::Index2D(0, 2, 4)] = static_cast<float>(X + Width);  // Top Right X
    Vertices[SnazzCraft::Index2D(1, 2, 4)] = static_cast<float>(Y);          // Top Right Y
    Vertices[SnazzCraft::Index2D(0, 3, 4)] = static_cast<float>(X + Width);  // Top Right X
    Vertices[SnazzCraft::Index2D(1, 3, 4)] = static_cast<float>(Y);          // Top Right Y
    Vertices[SnazzCraft::Index2D(0, 4, 4)] = static_cast<float>(X);          // Bottom Left X
    Vertices[SnazzCraft::Index2D(1, 4, 4)] = static_cast<float>(Y + Height); // Bottom Left Y
    Vertices[SnazzCraft::Index2D(0, 5, 4)] = static_cast<float>(X + Width);  // Bottom Right X
    Vertices[SnazzCraft::Index2D(1, 5, 4)] = static_cast<float>(Y + Height); // Bottom Right Y

    // Copy texture coordinate data
    for (uint8_t Y = 0x00; Y < 0x06; Y++) {
    for (uint8_t X = 0x02; X < 0x04; X++) {
        Vertices[SnazzCraft::Index2D<uint8_t>(X, Y, 4u)] = FullTextureCoordinates[SnazzCraft::Index2D<uint8_t>(X - 2, Y, 2u)];
    }
    }

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void SnazzCraft::Panel::Initiate(unsigned int X, unsigned int Y, unsigned int Width, unsigned int Height)
{
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    this->UpdateVertices(X, Y, Width, Height);

    glBindVertexArray(this->VAO);

    // Position attribute (location = 0)
    glVertexAttribPointer(
        0,                          // location
        2,                          // vec2
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float),          // STRIDE 
        (void*)0                    // offset
    );
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute (location = 1)
    glVertexAttribPointer(
        1,                          // location
        2,                          // vec2
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float),          // 
        (void*)(2 * sizeof(float))  // offset after x,y
    );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
