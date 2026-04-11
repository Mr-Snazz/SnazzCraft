#pragma once

#include <stdint.h>
#include <string>

#include "snazzcraft-engine/gui/panel/panel.hpp"
#include "snazzcraft-engine/texture/text-texture.hpp"

namespace SnazzCraft
{
    class Texture;

    class PanelWithTexture : public Panel
    {
        public:
            PanelWithTexture(float IX, float IY, float IWidth, float IHeight, float IScale, std::string IFilePathToTexture);

            virtual ~PanelWithTexture();

        protected:
            virtual void ProtectedDraw() const override;

            virtual void Initiate() override;

        private:
            std::string FilePathToTexture;
            SnazzCraft::Texture* Texture;

    };
} // SnazzCraft