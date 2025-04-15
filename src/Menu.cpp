#include "Menu.h"
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <raylib.h>

Menu::Menu(const char* title, const std::vector<const char*>& options) : m_options(options)
{
    m_title = title;

    m_screenWidth = GetScreenWidth();
    m_screenHeight = GetScreenHeight();
}

const char* Menu::DrawGameSelection()
{
    BeginDrawing();
    ClearBackground(SKYBLUE);

    constexpr int titleSize = 70;
    GuiSetStyle(DEFAULT, TEXT_SIZE, titleSize);

    const int titleWidth = MeasureText(m_title, titleSize);
    GuiLabel({m_screenWidth / 2.f - titleWidth / 2.f, 50, static_cast<float>(titleWidth), 100}, m_title);

    const char* subtitle = "select a game";
    constexpr int subtitleSize = 45;
    GuiSetStyle(DEFAULT, TEXT_SIZE, subtitleSize);
    const int subtitleWidth = MeasureText(subtitle, subtitleSize);
    GuiLabel({m_screenWidth / 2.f - subtitleWidth / 2.f, 120, static_cast<float>(subtitleWidth), 100}, subtitle);

    constexpr int buttonTextSize = 30;
    GuiSetStyle(DEFAULT, TEXT_SIZE, buttonTextSize);

    int result = -1;
    for (size_t i = 0; i < m_options.size(); ++i)
    {
        constexpr float buttonWidth = 150.f;
        constexpr float buttonHeight = 50.f;
        const float buttonX = m_screenWidth / 2.f - buttonWidth / 2.f;
        const float buttonY = m_screenHeight / 2.f + i * (buttonHeight + 10.f);

        if (GuiButton({buttonX, buttonY, buttonWidth, buttonHeight}, m_options[i]))
        {
            result = static_cast<int>(i);
            break;
        }
    }

    EndDrawing();

    if (result != -1)
    {
        return m_options[result];
    }

    return nullptr;
}
