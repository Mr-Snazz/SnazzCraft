#pragma once

#include <vector>

namespace SnazzCraft
{
    class Panel;
    class Button;

    struct WidgetCollection
    {
    public:
        std::vector<SnazzCraft::Panel*> Panels;
        std::vector<SnazzCraft::Button*> Buttons;

        WidgetCollection() = default;

        ~WidgetCollection();

        void Draw(); // Panels are drawn before buttons

    private:

    };
} // SnazzCraft


