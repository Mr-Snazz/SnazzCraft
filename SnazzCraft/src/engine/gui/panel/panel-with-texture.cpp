#include "snazzcraft-engine/gui/panel/panel-with-texture.hpp"
#include "snazzcraft-engine/texture/texture.hpp"
#include "snazzcraft-engine/utilities/math.hpp"

SnazzCraft::PanelWithTexture::PanelWithTexture(float IX, float IY, float IWidth, float IHeight, float IScale, std::string IFilePathToTexture)
    : Panel(IX, IY, IWidth, IHeight, IScale), FilePathToTexture(IFilePathToTexture), Texture(nullptr)
{
    this->Initiate();
}

SnazzCraft::PanelWithTexture::~PanelWithTexture()
{
    delete this->Texture;
}

void SnazzCraft::PanelWithTexture::ProtectedDraw() const
{
    if (this->Texture == nullptr || !this->Texture->BindTexture()) return;
 
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SnazzCraft::PanelWithTexture::Initiate()
{
    delete this->Texture;
    this->Texture = new SnazzCraft::Texture(this->FilePathToTexture);

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    this->SetVertices();

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