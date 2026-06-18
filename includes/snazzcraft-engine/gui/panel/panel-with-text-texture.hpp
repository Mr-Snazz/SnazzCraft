#pragma once

#include "snazzcraft-engine/gui/panel/panel-with-texture.hpp"

namespace SnazzCraft
{
    class TextTexture;

    class PanelWithTextTexture : public Panel
    {
    public:
        PanelWithTextTexture(float IX, float IY, std::string IText, uint8_t IR, uint8_t IG, uint8_t IB, float IScale);

        PanelWithTextTexture(float IX, float IY, std::string IText, uint8_t IR, uint8_t IG, uint8_t IB, float IScale, uint8_t IPerCharacterWidth, uint8_t IPerCharacterHeight);

        PanelWithTextTexture(float IX, float IY, float IScale, SnazzCraft::TextTexture* ITexture);

        virtual ~PanelWithTextTexture();

    protected:
        virtual void ProtectedDraw() const override;

        virtual void Initiate() override;

    private:
    
        SnazzCraft::TextTexture* Texture;
    };


} // SnazzCraft
