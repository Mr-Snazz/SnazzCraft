#pragma once

#include "glad.h"

#include "snazzcraft-engine/texture/texture.hpp"

#define SNAZZCRAFT_PANEL_LONG_WIDTH   (275)
#define SNAZZCRAFT_PANEL_SHORT_WIDTH  (125)

#define SNAZZCRAFT_PANEL_LONG_HEIGHT  (50)
#define SNAZZCRAFT_PANEL_SHORT_HEIGHT (50)

namespace SnazzCraft
{
    class Texture;

    class Panel
    {
    public:
        SnazzCraft::Texture* Texture = nullptr;

        Panel(unsigned int X, unsigned int Y, unsigned int Width, unsigned int Height, SnazzCraft::Texture* Texture); // NOTE: Texture will be deleted when destructor is called

        ~Panel();

        void Draw();

        void UpdateVertices(unsigned int X, unsigned int Y, unsigned int Width, unsigned int Height);

        inline void SwapTextures(SnazzCraft::Texture* NewTexture) // Deletes currently bound texture
        {
            delete this->Texture;
            this->Texture = NewTexture;
        }

    private:
        unsigned int VAO;
        unsigned int VBO;

        void Initiate(unsigned int X, unsigned int Y, unsigned int Width, unsigned int Height);

    public:
        static inline SnazzCraft::Panel* NewPanelLong(uint32_t X, uint32_t Y, SnazzCraft::Texture* Texture)
        {
            return new SnazzCraft::Panel(X, Y, SNAZZCRAFT_PANEL_LONG_WIDTH, SNAZZCRAFT_PANEL_LONG_HEIGHT, Texture);
        }

        static inline SnazzCraft::Panel* NewPanelShort(uint32_t X, uint32_t Y, SnazzCraft::Texture* Texture)
        {
            return new SnazzCraft::Panel(X, Y, SNAZZCRAFT_PANEL_SHORT_WIDTH, SNAZZCRAFT_PANEL_SHORT_HEIGHT, Texture);
        }

    };
} // SnazzCraft



