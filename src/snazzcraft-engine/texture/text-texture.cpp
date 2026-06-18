#include "snazzcraft-engine/texture/text-texture.hpp"

SnazzCraft::TextTexture::TextTexture(std::string IText, uint8_t IR, uint8_t IG, uint8_t IB)
    : Texture(), Text(IText), R(IR), G(IG), B(IB), PerCharacterWidth(0x08), PerCharacterHeight(0x08)
{
    this->Update();
}

SnazzCraft::TextTexture::TextTexture(std::string IText, uint8_t IR, uint8_t IG, uint8_t IB, uint8_t IPerCharacterWidth, uint8_t IPerCharacterHeight)
    : Texture(), Text(IText), R(IR), G(IG), B(IB), PerCharacterWidth(IPerCharacterWidth), PerCharacterHeight(IPerCharacterHeight)
{
    this->Update();
}

void SnazzCraft::TextTexture::Update()
{
    this->DeleteBoundData();

    this->Width  = static_cast<uint32_t>(Text.length()) * this->PerCharacterWidth;
    this->Height = this->PerCharacterHeight;

    uint32_t TotalSize = this->Width * this->Height * static_cast<uint32_t>(this->NRChannels);
    this->Data = new uint8_t[TotalSize];
    
    for (uint32_t I = 0; I < TotalSize; I++) this->Data[I] = 0x00;

    for (uint32_t CharIndex = 0; CharIndex < Text.length(); CharIndex++) {
        char Character = std::tolower(Text[CharIndex]);

        const std::string FilePath = [Character]() -> std::string
        {
            switch (Character)
            {
            case ' ':
                return "textures/gui/font/space.txt";
            
            default:
                return "textures/gui/font/" + std::string(1, Character) + ".txt";
            }
        }();
        
        std::ifstream CharacterFile(FilePath);
        if (!CharacterFile.is_open()) continue;

        for (uint32_t Y = 0; Y < this->PerCharacterHeight; Y++) {
            std::string Line;
            if (!std::getline(CharacterFile, Line)) break;

            uint32_t V = Y;
            
            for (uint32_t X = 0; X < this->PerCharacterWidth; X++) {
                uint32_t U = X;

                if (U >= Line.length() || Line[U] != '1') continue;

                uint32_t GlobalU = (CharIndex * this->PerCharacterWidth) + U;
                uint32_t Index = (V * this->Width + GlobalU) * this->NRChannels;

                if (this->NRChannels >= 1) this->Data[Index + 0] = this->R;
                if (this->NRChannels >= 2) this->Data[Index + 1] = this->G;
                if (this->NRChannels >= 3) this->Data[Index + 2] = this->B;
                if (this->NRChannels >= 4) this->Data[Index + 3] = this->Alpha;
            }
        }
        CharacterFile.close();
    }

    this->ID = new uint32_t;
    this->SetTexture();
}


