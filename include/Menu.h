#pragma once

#include <vector>

class Menu
{
public:
    explicit Menu(const char* title, const std::vector<const char*>& options);
    const char* DrawGameSelection();

private:
    const char* m_title = nullptr;
    std::vector<const char*> m_options;

    float m_screenWidth = 0;
    float m_screenHeight = 0;
};