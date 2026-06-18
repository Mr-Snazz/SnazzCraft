#include "snazzcraft-engine/texture/text-texture.hpp"

inline uint32_t SnazzCraft::TextTexture::GetWidth() const
{
    return this->Text.size() * this->PerCharacterWidth;
}

inline bool SnazzCraft::TextTexture::BindTexture() const 
{
    if (this->ID == nullptr) return false;

    glBindTexture(GL_TEXTURE_2D, *this->ID);
    return true;
}

