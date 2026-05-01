#include "snazzcraft-engine/gui/panel/panel.hpp"
#include "snazzcraft-engine/utilities/math.hpp"

#include "external/glad.h"
#include "snazzcraft-engine/input-handler/event.hpp"

#include "snazzcraft-engine/input-handler/data-types.h"
#include "snazzcraft-engine/core/core-callback.hpp"

constexpr float FullTextureCoordinates[12] = {
    0.0f, 1.0f, // Bottom Left
    0.0f, 0.0f, // Top Left
    1.0f, 0.0f, // Top Right
    1.0f, 0.0f, // Top Right
    0.0f, 1.0f, // Bottom Left
    1.0f, 1.0f  // Bottom Right
};

void SnazzCraft::Panel::HandleEvent(SnazzCraft::Event* Event) const
{
    if (this->Callback == nullptr) return;

    glm::dvec2* MousePosition = reinterpret_cast<glm::dvec2*>(Event->EventData->AccessDataType(SNAZZCRAFT_DATA_TYPE_DVEC2));
    if (MousePosition == nullptr || !this->WithinPanel(MousePosition->x, MousePosition->y)) return;
    
    this->Callback(Event);
}

SnazzCraft::Panel::~Panel()
{
    this->DeleteBoundData();
}

SnazzCraft::Panel::Panel(float IX, float IY, float IWidth, float IHeight, float IScale)
    : X(IX), Y(IY), Width(IWidth), Height(IHeight), Scale(IScale), Callback(nullptr)
{
    
    
}

void SnazzCraft::Panel::SetVertices()
{
    glBindVertexArray(this->VAO);

    float Vertices[24];
    // Copy position data
    Vertices[SnazzCraft::Index2D(0, 0, 4)] = X ;                         // Bottom Left X
    Vertices[SnazzCraft::Index2D(1, 0, 4)] = Y + (Height * this->Scale); // Bottom Left Y

    Vertices[SnazzCraft::Index2D(0, 1, 4)] = X;                          // Top Left X
    Vertices[SnazzCraft::Index2D(1, 1, 4)] = Y;                          // Top Left Y

    Vertices[SnazzCraft::Index2D(0, 2, 4)] = X + (Width  * this->Scale); // Top Right X
    Vertices[SnazzCraft::Index2D(1, 2, 4)] = Y;                          // Top Right Y

    Vertices[SnazzCraft::Index2D(0, 3, 4)] = X + (Width  * this->Scale); // Top Right X
    Vertices[SnazzCraft::Index2D(1, 3, 4)] = Y;                          // Top Right Y

    Vertices[SnazzCraft::Index2D(0, 4, 4)] = X;                          // Bottom Left X
    Vertices[SnazzCraft::Index2D(1, 4, 4)] = Y + (Height * this->Scale); // Bottom Left Y

    Vertices[SnazzCraft::Index2D(0, 5, 4)] = X + (Width  * this->Scale); // Bottom Right X
    Vertices[SnazzCraft::Index2D(1, 5, 4)] = Y + (Height * this->Scale); // Bottom Right Y

    // Copy texture coordinate data
    for (uint8_t Y = 0x00; Y < 0x06; Y++) {
    for (uint8_t X = 0x02; X < 0x04; X++) {
        Vertices[SnazzCraft::Index2D<uint8_t>(X, Y, 4u)] = FullTextureCoordinates[SnazzCraft::Index2D<uint8_t>(X - 2, Y, 2u)];
    }
    }

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    
}

void SnazzCraft::Panel::ProtectedDraw() const
{
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SnazzCraft::Panel::Initiate()
{

}
