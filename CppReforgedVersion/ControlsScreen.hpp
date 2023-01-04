#pragma once

#include "Screen.hpp"
#include "ScreenRenderer.hpp"

#include <string>
#include <vector>


class ControlsScreen : public Screen
{
public:
    //              [CONSTRUCTORS]

    ControlsScreen()
        : Screen{ 30 }
    {
        // Runtime evaluation of a position of the text on the screen. Was adjusted manually

        // Maximum size of the screen in terminal mesuared in 'character places'
        Vec2 maxPos { ScreenRenderer::GetMaxSize() };
        Vec2 currentPos { maxPos.x / 2, 2 };

        // Helpful lambda for calculating the center position of the string
        auto CenterPosOfString = [](const Vec2& pos, const std::string& str) -> Vec2
        {
            return { pos.x - static_cast<int>(str.length()) / 2, pos.y };
        };

        _textToRender.push_back({ _titleText, CenterPosOfString(currentPos, _titleText) });

        currentPos.y += 2;
        _textToRender.push_back({ _upArrowText, CenterPosOfString(currentPos, _upArrowText) });

        currentPos.y += 1;
        _textToRender.push_back({ _downArrowText, CenterPosOfString(currentPos, _downArrowText) });

        currentPos.y += 1;
        _textToRender.push_back({ _leftArrowText, CenterPosOfString(currentPos, _leftArrowText) });

        currentPos.y += 1;
        _textToRender.push_back({ _rightArrowText, CenterPosOfString(currentPos, _rightArrowText) });

        currentPos.y += 2;
        _textToRender.push_back({ _boostText, CenterPosOfString(currentPos, _boostText) });

        currentPos.y += 2;
        _textToRender.push_back({ _escapeText, CenterPosOfString(currentPos, _escapeText) });

        currentPos.y = maxPos.y - 2;
        _textToRender.push_back({ _returnBackText, CenterPosOfString(currentPos, _returnBackText) });

        // '2' is an offset from 'returnBack' text on X axis
        _textToRender.push_back({ _selectionMark, { _textToRender.back().position.x - 2, _textToRender.back().position.y } });
    }


    //              [PUBLIC METHODS]

    ActiveState Run() override
    {
        Draw();

        UserInput::Key key{ UserInput::Undefined };
        while( (key = UserInput::GetKey()) != UserInput::Key::Enter)
        {
            Screen::WaitForUpdate();
        }

        return ActiveState::MenuScreen;
    }

private:
    //              [PRIVATE VARIABLES]

    // Text constants

    const std::string _titleText{ "Controls" };
    const std::string _upArrowText { "Up arrow - moving up" };
    const std::string _downArrowText { "Down arrow - moving down" };
    const std::string _leftArrowText { "Left arrow - moving left" };
    const std::string _rightArrowText { "Right arrow - moving right" };
    const std::string _boostText { "press and hold move key to boost up your snake" };
    const std::string _escapeText { "pressing ESC returns you to the main menu" };
    const std::string _returnBackText { "back" };
    const std::string _selectionMark { '>' };


    // Useful storage for rendering the text. If you want to add new text on the screen,
    // simply add 'const std::string', calculate the position of the text, create 'struct TextField'
    // using these two parameters and add this 'struct TextField' to this array.
    // 'void Draw()' will automatically render the new text.
    std::vector<TextField> _textToRender;


    //              [PRIVATE METHODS]

    // Draws the whole screen
    void Draw() override
    {
        ScreenRenderer::EraseScreen();

        // Draws all the screen text
        for( const auto& [text, textPosition] : _textToRender)
            ScreenRenderer::DrawText(textPosition, text);
    }

    void ProcessInput(UserInput::Key key) override
    {}
};