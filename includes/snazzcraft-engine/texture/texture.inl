#include "snazzcraft-engine/texture/texture.hpp"

inline bool SnazzCraft::Texture::BindTexture() const
{
    if (this->ID == nullptr) return false;

    glBindTexture(GL_TEXTURE_2D, *this->ID);
    return true;
}

inline void SnazzCraft::Texture::DeleteBoundData()
{
    if (this->Data == nullptr) return;

    delete this->Data;
    glDeleteTextures(1, this->ID);
}

