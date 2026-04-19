#pragma once

#include <stdint.h>

#include "glad.h"

namespace SnazzCraft
{   
    class Event;

    class Panel
    {
    public:  
        virtual void HandleEvent(SnazzCraft::Event* Event) const;

        inline void Draw() const
        {
            this->ProtectedDraw();
        }

        inline bool WithinPanel(double X, double Y) const
        {
            return X >= this->X && X <= this->X + (this->Width  * this->Scale) && 
                   Y >= this->Y && Y <= this->Y + (this->Height * this->Scale);
        }

        inline void SetCallback(void(*NewCallback)(SnazzCraft::Event* Event))
        {
            this->Callback = NewCallback;
        }

        inline float GetWidth() const
        {
            return this->Width * this->Scale;
        }

        inline float GetHeight() const
        {
            return this->Height * this->Scale;
        }

        inline float GetX() const
        {
            return this->X;
        }

        inline float GetY() const
        {
            return this->Y;
        }

        virtual ~Panel();

    protected:
        union 
        {
            struct 
            {
                float X, Y;
            };
            float Position[2];
        };

        union 
        {
            struct 
            {
                float Width, Height;
            };
            float Dimensions[2];
        };

        float Scale;

        uint32_t VAO, VBO;
        void(*Callback)(SnazzCraft::Event* Event);
    
        Panel(float IX, float IY, float IWidth, float IHeight, float IScale);

        virtual void SetVertices(); 

        virtual void ProtectedDraw() const;

        virtual void Initiate();

        virtual inline void DeleteBoundData()
        {
            glDeleteVertexArrays(1, &this->VAO);
            glDeleteBuffers(1, &this->VBO);
        }

    private:


    };
};