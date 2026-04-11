#pragma once

#include <stdint.h>

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
            return X >= static_cast<float>(this->X) && X <= static_cast<float>(this->X + this->Width) && 
                Y >= static_cast<float>(this->Y) && Y <= static_cast<float>(this->Y + this->Height);
        }

        inline void SetCallback(void(*NewCallback)(SnazzCraft::Event* Event))
        {
            this->Callback = NewCallback;
        }

        virtual ~Panel();

    protected:
        union 
        {
            struct 
            {
                uint8_t X, Y;
            };
            uint8_t Position[2];
        };

        union 
        {
            struct 
            {
                uint32_t Width, Height;
            };
            uint32_t Dimensions[2];
        };

        /*
        Vertice:
            X, Y, U, V
        */
        uint32_t VAO, VBO;
        void(*Callback)(SnazzCraft::Event* Event);
    
        Panel(uint8_t IX, uint8_t IY, uint32_t IWidth, uint32_t IHeight);

        virtual void SetVertices(); 

        virtual void ProtectedDraw() const;

        virtual void Initiate();

    private:


    };
};