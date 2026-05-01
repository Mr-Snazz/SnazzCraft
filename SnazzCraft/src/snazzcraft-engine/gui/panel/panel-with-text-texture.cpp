#include "snazzcraft-engine/gui/panel/panel-with-text-texture.hpp"
#include "snazzcraft-engine/texture/text-texture.hpp"

#include "external/glad.h"

SnazzCraft::PanelWithTextTexture::PanelWithTextTexture(float IX, float IY, std::string IText, uint8_t IR, uint8_t IG, uint8_t IB, float IScale)
    : Panel(IX, IY, 0.0f, 0.0f, IScale)
{
    this->Texture = new SnazzCraft::TextTexture(IText, IR, IG, IB);

    this->Initiate();
}

SnazzCraft::PanelWithTextTexture::PanelWithTextTexture(float IX, float IY, std::string IText, uint8_t IR, uint8_t IG, uint8_t IB, float IScale, uint8_t IPerCharacterWidth, uint8_t IPerCharacterHeight)
    : Panel(IX, IY, 0.0f, 0.0f, IScale)
{
    this->Texture = new SnazzCraft::TextTexture(IText, IR, IG, IB, IPerCharacterWidth, IPerCharacterHeight);

    this->Initiate();
}

SnazzCraft::PanelWithTextTexture::PanelWithTextTexture(float IX, float IY, float IScale, SnazzCraft::TextTexture* ITexture)
    : Panel(IX, IY, 0.0f, 0.0f, IScale), Texture(ITexture)
{
    this->Initiate();
}

SnazzCraft::PanelWithTextTexture::~PanelWithTextTexture()
{
    delete this->Texture;
}

void SnazzCraft::PanelWithTextTexture::ProtectedDraw() const
{
    if (this->Texture == nullptr || !this->Texture->BindTexture()) return;
 
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SnazzCraft::PanelWithTextTexture::Initiate()
{
    this->Width  = this->Texture->Width;
    this->Height = this->Texture->Height;

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


