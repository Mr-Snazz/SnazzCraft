#pragma once

#include "snazzcraft-engine/texture/texture.hpp"

namespace SnazzCraft
{
    class TextTexture : public Texture
    {
    public:
        std::string Text;

        union 
        {
            struct 
            {
                uint8_t R, G, B;
            };  
            uint8_t TextColor[3];
        };
        uint8_t Alpha = 0xFF;

        union 
        {
            struct 
            {
                uint8_t PerCharacterWidth, PerCharacterHeight;
            };
            uint8_t PerCharacterDimensions[2];
        };

        TextTexture(std::string IText, uint8_t IR, uint8_t IG, uint8_t IB);

        TextTexture(std::string IText, uint8_t IR, uint8_t IG, uint8_t IB, uint8_t IPerCharacterWidth, uint8_t IPerCharacterHeight);

        virtual ~TextTexture() = default;

        virtual void Update();

        inline uint32_t GetWidth() const;

        virtual inline bool BindTexture() const override;

    protected:
        
        
    private:


    public:



    };
} // SnazzCraft

#include "snazzcraft-engine/texture/text-texture.inl"

