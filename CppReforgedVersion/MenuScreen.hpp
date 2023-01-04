#pragma once

#include "Screen.hpp"
#include "ScreenRenderer.hpp"
#include "UserInput.hpp"
#include "GlobalTypes.hpp"

#include <vector>

// [TODO]: refactor in the way of ControlsScreen class
class MenuScreen : public Screen
{
public:
    //              [NEW TYPES]

    enum MenuItem
    {
        Start,
        Controls,
        Exit,

        MaxItemCount
    };


    //              [CONSTRUCTORS]

    MenuScreen()
        : Screen{ 60 }
        , _itemPositions(MenuItem::MaxItemCount)
        , _currentState { ActiveState::MenuScreen }
    {
        // The whole point of this constructor is to calculate interface element's positions and store them in variables

	    Vec2 maxScreenSize;
        Vec2 curPos;

	    maxScreenSize = ScreenRenderer::GetMaxSize();

	    curPos.x = maxScreenSize.x / 2;
	    curPos.y = maxScreenSize.y / 6;

        _titlePos = { curPos.x - static_cast<int>(std::string(_gameTitle).length()) / 2, curPos.y };

	    curPos.y += 2;
        _authorPos = { curPos.x - static_cast<int>(std::string(_developedBy).length() / 2), curPos.y };

        Vec2 startButtonPos;
        Vec2 controlsButtonPos;
        Vec2 exitButtonPos;

	    curPos.y = maxScreenSize.y / 2 - 1;
	    startButtonPos.x = curPos.x - std::string(_startButtonText).length() / 2;
	    startButtonPos.y = curPos.y;

	    curPos.y += 1;
	    controlsButtonPos.x = curPos.x - std::string(_controlsButtonText).length() / 2;
	    controlsButtonPos.y = curPos.y;

	    curPos.y += 1;
	    exitButtonPos.x = curPos.x - std::string(_exitButtonText).length() / 2;
	    exitButtonPos.y = curPos.y;

        _itemPositions[MenuItem::Start] = startButtonPos;
        _itemPositions[MenuItem::Controls] = controlsButtonPos;
        _itemPositions[MenuItem::Exit] = exitButtonPos;

        _currentItem = MenuItem::Start;
        _selectionMark.SetPosition(_itemPositions[_currentItem]);
    }


    //              [PUBLIC METHODS]

    ActiveState Run() override
    {
        Draw();

        _currentState = ActiveState::MenuScreen;

        while(_currentState == ActiveState::MenuScreen)
        {
            ProcessInput(UserInput::GetKey());

            Screen::WaitForUpdate();
        }

        return _currentState;
    }

private:
    //              [NEW TYPES]

    class SelectionMark
    {
    public:
        SelectionMark() = default;
        SelectionMark(Vec2 pos)
        {
            currentPos = { pos.x - markIndent, pos.y };

            // Semantically invalid, but helpful with the first call to 'void Draw()'
            // so there is no effect of removal of the last printed symbol,
            // because valid screen coordinates are: 0 < x < maxSize.x (same for Y axis)
            lastPos = { -1, -1 };
        }

        void SetPosition(Vec2 newPos)
        {
            lastPos = currentPos;
            currentPos = { newPos.x - markIndent, newPos.y };
        }

        void Draw()
        {
            // Deletes the previous printed symbol
            ScreenRenderer::DrawChar(lastPos, ' ');

            ScreenRenderer::DrawChar(currentPos, markSymbol);
        }

    private:
        const char markSymbol = '>';
        const int markIndent = 2;
        Vec2 currentPos{};
        Vec2 lastPos{};
    };


    //              [PRIVATE VARIABLES]

    const std::string _gameTitle { "Snake!" };
    const std::string _developedBy {"developed by German"};
    const std::string _startButtonText {"start"};
    const std::string _controlsButtonText {"how to play"};
    const std::string _exitButtonText { "exit" };

    std::vector<Vec2> _itemPositions;
    Vec2 _titlePos;
    Vec2 _authorPos;

    MenuItem _currentItem;
    ActiveState _currentState;
    SelectionMark _selectionMark;


    //              [PRIVATE METHODS]

    // Draws a menu screen using precomputed variables
    void Draw() override
    {
        ScreenRenderer::EraseScreen();

        // Prints the title "Snake!"
	    ScreenRenderer::DrawText(_titlePos, _gameTitle);

        // Prints the author's name
	    ScreenRenderer::DrawText(_authorPos, _developedBy);

        // Prints the start button
	    ScreenRenderer::DrawText(_itemPositions[MenuItem::Start], _startButtonText);

        // Print the controls button
	    ScreenRenderer::DrawText(_itemPositions[MenuItem::Controls], _controlsButtonText);

        // Prints the exit button
	    ScreenRenderer::DrawText(_itemPositions[MenuItem::Exit], _exitButtonText);

        // Prints the menu's item selection mark
        _selectionMark.Draw();
    }

    void ProcessInput(UserInput::Key key) override
    {
        int currItem { _currentItem };
        int maxItemCount { MenuItem::MaxItemCount };
        switch(key)
        {
        case UserInput::Key::DownArrow:
            currItem++;
            if(currItem >= maxItemCount)
                currItem %= maxItemCount;
            _currentItem = static_cast<MenuItem>(currItem);
            _selectionMark.SetPosition(_itemPositions[_currentItem]);
            _selectionMark.Draw();
        break;

        case UserInput::Key::UpArrow:
            currItem--;
            if(currItem < 0)
                currItem = maxItemCount - 1;
            _currentItem = static_cast<MenuItem>(currItem);
            _selectionMark.SetPosition(_itemPositions[_currentItem]);
            _selectionMark.Draw();
        break;

        case UserInput::Key::Enter:
            switch(_currentItem)
            {
                case MenuItem::Start:
                    _currentState = ActiveState::GameScreen;
                break;

                case MenuItem::Controls:
                    _currentState = ActiveState::ControlsScreen;
                break;

                case MenuItem::Exit:
                    _currentState = ActiveState::Exit;
                return;
            }
        break;

        case UserInput::Key::Escape:
            _currentState = ActiveState::Exit;
        break;

        default:
            return;
        }
    }
};