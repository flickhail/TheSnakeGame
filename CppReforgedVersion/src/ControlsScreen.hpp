#pragma once

#include "Screen.hpp"
#include "TerminalAPI.hpp"
#include "MenuScreen.hpp"

#include <string>
#include <vector>

class ControlsScreen : public Screen
{
public:
    explicit ControlsScreen(int updateFrequency)
        : Screen{ updateFrequency }
        , m_selectionMark{ Vec2{0, 0}, Screen::screenWindow }
    {
        // Runtime evaluation of a position of the text on the screen. Was adjusted manually

        // Maximum size of the screen in terminal mesuared in 'character places'
        Vec2 maxPos { Renderer::GetMaxSize() };
        Vec2 currentPos { maxPos.x / 2, 2 };

        // Helpful lambda for calculating the center position of the string
        auto StrCentr = [](const Vec2& pos, const std::string& str) -> Vec2
        {
            return { pos.x - static_cast<int>(str.length()) / 2, pos.y };
        };

        m_staticText.push_back({ TITLE_TEXT, StrCentr(currentPos, TITLE_TEXT) });

        currentPos.y += 2;
        m_staticText.push_back({ UP_ARROW_TEXT, StrCentr(currentPos, UP_ARROW_TEXT) });

        currentPos.y += 1;
        m_staticText.push_back({ DOWN_ARROW_TEXT, StrCentr(currentPos, DOWN_ARROW_TEXT) });

        currentPos.y += 1;
        m_staticText.push_back({ LEFT_ARROW_TEXT, StrCentr(currentPos, LEFT_ARROW_TEXT) });

        currentPos.y += 1;
        m_staticText.push_back({ RIGHT_ARROW_TEXT, StrCentr(currentPos, RIGHT_ARROW_TEXT) });

        currentPos.y += 2;
        m_staticText.push_back({ BOOST_TEXT, StrCentr(currentPos, BOOST_TEXT) });

        currentPos.y += 2;
        m_staticText.push_back({ ESCAPE_TEXT, StrCentr(currentPos, ESCAPE_TEXT) });

        currentPos.y = maxPos.y - 2;
        m_staticText.push_back({ RETURN_BACK_TEXT, StrCentr(currentPos, RETURN_BACK_TEXT) });
        m_staticText.shrink_to_fit();

        m_selectionMarkPos = m_staticText.back().position;

        StaticDraw();
        ResetState();
    }

    // Draws the whole screen
    void StaticDraw()
    {
        Screen::ClearScreen();

        // Draws all the static screen text
        for( const auto& textField : m_staticText)
            Screen::screenWindow.PrintText(textField.position, textField.text);
    }

    void UpdateScreenBuffer() override
    {
        Screen::screenWindow.Refresh();
    }

    void ProcessInput(UserInput::Key key) override
    {
        switch(key)
        {
        case UserInput::Key::Enter:
            Screen::shouldClose = true;
            Screen::eventListener->onEvent(ScreenChangeEvent{ ScreenType::MainMenu });
        break;
        }
    }

    void Tick(float deltaTime) override
    {
        static float time{ 0 };
        constexpr float blinkInterval{ 0.45f };

        time += deltaTime;

        if(time > blinkInterval)
        {
            m_selectionMark.SetVisibility(!m_selectionMark.isVisible);
            time = 0;
        }
    }

    void ResetState() override
    {
        Screen::ResetState();
        m_selectionMark.SetPosition(m_selectionMarkPos);
    }

private:
    const std::string TITLE_TEXT{ "Controls" };
    const std::string UP_ARROW_TEXT { "Up arrow - moving up" };
    const std::string DOWN_ARROW_TEXT { "Down arrow - moving down" };
    const std::string LEFT_ARROW_TEXT { "Left arrow - moving left" };
    const std::string RIGHT_ARROW_TEXT { "Right arrow - moving right" };
    const std::string BOOST_TEXT { "press and hold move key to boost up your snake" };
    const std::string ESCAPE_TEXT { "pressing ESC returns you to the main menu" };
    const std::string RETURN_BACK_TEXT { "back" };
    
    SelectionMark m_selectionMark;
    Vec2 m_selectionMarkPos;

    // Useful storage for rendering the text. If you want to add new text on the screen,
    // simply add 'const std::string', calculate the position of the text, create 'struct TextField'
    // using these two parameters and add this 'struct TextField' to this array.
    // 'void Draw()' will automatically render the new text.
    std::vector<TextField> m_staticText;
};