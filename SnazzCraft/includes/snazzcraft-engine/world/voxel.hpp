#pragma once

#include <vector>
#include <stdint.h>

#include "glm/glm.hpp"
#include "snazzcraft-engine/world/voxel-ids.h"

#define MAX_VOXEL_LIGHT_VALUE (20)

namespace SnazzCraft
{
    class Voxel
    {
    public:
        static constexpr uint32_t Size = 2;

        union 
        {
            struct 
            {
                uint32_t X, Y, Z;
            };
            uint32_t Position[3]; // In local chunk space
        };
        uint32_t ID;

        // Specific values ->
        int LightProducingLevel = 0;
        int LightPropogationDecrease = MAX_VOXEL_LIGHT_VALUE;
        bool Cullable = true;
        bool CollidableToEntities = true;
        bool CollidableToVoxels = true;
        // <-

        Voxel(uint32_t IX, uint32_t IY, uint32_t IZ, uint32_t IID);

        inline void AutoSetSpecificValues()
        {
            switch (this->ID)
            {
                case ID_VOXEL_ABOVE_GRASS:
                    this->LightProducingLevel = 0;
                    this->LightPropogationDecrease = 0;

                    this->Cullable = false;
                    this->CollidableToEntities = false;
                    this->CollidableToVoxels = true;
                    break;

                case ID_VOXEL_TORCH:
                    this->LightProducingLevel = 18;
                    this->LightPropogationDecrease = 0;
                    this->Cullable = false;
                    this->CollidableToEntities = false;
                    this->CollidableToVoxels = true;
                    break;

                default:
                    this->LightProducingLevel = 0;
                    this->LightPropogationDecrease = MAX_VOXEL_LIGHT_VALUE;
                    this->Cullable = true;
                    this->CollidableToEntities = true;
                    this->CollidableToVoxels = true;
                    break;
            }
        }

        inline bool HasSide(uint8_t SideIndex) const
        {
            if (SideIndex > 5) return false;
            return (this->Sides >> SideIndex) & 1;
        }

        inline void ChangeSideValue(uint8_t SideIndex, bool Value)
        {
            if (SideIndex > 5) return;
            if (Value) {
                this->Sides |= (1 << SideIndex);
            } else {
                this->Sides &= ~(1 << SideIndex);
            }
        }

        inline void SetAllSides()
        {
            this->Sides = 0x3F;
        }

        inline void ClearAllSides()
        {
            this->Sides = 0x00;
        }

        inline uint8_t GetSideCount() const
        {
            uint8_t Count = 0;
            for (uint8_t I = 0; I < 6; I++) {
                if (this->HasSide(I)) Count++;
            }

            return Count;
        }

    private:
        // Bit Order:
        // false, false, Bottom, Top, Back, Right, Left, Front
        uint8_t Sides = 0x3F;

    };
} // SnazzCraft

