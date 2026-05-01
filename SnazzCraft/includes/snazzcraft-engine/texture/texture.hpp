#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <stdint.h>

#include "external/glad.h"
#include "external/stb_image.h"
#include "external/glm/glm.hpp"

#define SNAZZCRAFT_CHARACTER_BITWISE_WIDTH  (8)
#define SNAZZCRAFT_CHARACTER_BITWISE_HEIGHT (8)

namespace SnazzCraft
{
    class Texture
    {
    public:
        union 
        {
            struct 
            {
                int32_t Width, Height;
            };
            int32_t Dimensions[2];
        };

        uint8_t* Data = nullptr;
        int32_t NRChannels = 4;
        uint32_t* ID = nullptr;

        uint32_t WrapS = GL_REPEAT;
        uint32_t WrapT = GL_REPEAT;

        Texture(std::string FilePath);

        Texture(uint32_t IWidth, int32_t IHeight, uint8_t* IData, int32_t INRChannels, uint32_t* IID);

        virtual ~Texture();

        void LoadTexture(std::string FilePath);

        virtual inline bool BindTexture() const
        {
            if (this->ID == nullptr) return false;

            glBindTexture(GL_TEXTURE_2D, *this->ID);
            return true;
        }

    protected:
        Texture(); // Emmediatly load texture for newly created object for safety

        void SetTexture();
        
        inline void DeleteBoundData()
        {
            if (this->Data == nullptr) return;

            delete this->Data;
            glDeleteTextures(1, this->ID);
        }
    };

    extern SnazzCraft::Texture* VoxelTextureAtlas;
} // SnazzCraft



