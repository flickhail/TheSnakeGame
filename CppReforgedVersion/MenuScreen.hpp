#pragma once

#include "Screen.hpp"
#include "ScreenRenderer.hpp"
#include "UserInput.hpp"
#include "Logger.hpp"

#include <vector>

class SelectionMark : public ILogPrintable
    {
    public:
        bool isVisible{ true };

        SelectionMark() = delete;
        SelectionMark(Vec2 pos)
        : isVisible{ true }
        {
            currentPos = { pos.x - markIndent, pos.y };
            Renderer::DrawChar(currentPos, markSymbol);

            // Semantically invalid, but helpful with the first call to 'void Draw()'
            // so there is no effect of removal of the last printed symbol,
            // because valid screen coordinates are: 0 < x < maxSize.x (same for Y axis)
            lastPos = { -1, -1 };
        }


        void SetPosition(Vec2 newPos)
        {
            if(isVisible)
            {
                lastPos = currentPos;
                currentPos = { newPos.x - markIndent, newPos.y };
                Renderer::DrawChar(lastPos, ' ');
                Renderer::DrawChar(currentPos, markSymbol);
            }
            else
            {
                currentPos = { newPos.x - markIndent, newPos.y };
            }
        }

        void Hide()
        {
            Renderer::DrawChar(currentPos, ' ');
            isVisible = false;
        }

        void Show()
        {
            Renderer::DrawChar(currentPos, markSymbol);
            isVisible = true;
        }

        void SetVisibility(bool value)
        {
            isVisible = value;

            if(isVisible)
                Show();
            else
                Hide();
        }

        std::string ToString() const override
        {
            std::string tmp{ "SelectionMark pos: " + std::to_string(currentPos.x) + ' ' + std::to_string(currentPos.y) };
            return tmp;
        }

    private:
        const char markSymbol = '>';
        const int markIndent = 2;
        Vec2 currentPos{};
        Vec2 lastPos{};
    };

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

    explicit MenuScreen(int updateFrequency)
        : Screen{ updateFrequency }
        , _itemPositions{ MenuItem::MaxItemCount }
        , _selectionMark{ Vec2{0, 0} }
    {
        // The whole point of this constructor is to calculate interface element's positions and store them in variables

	    Vec2 maxScreenSize{ Renderer::GetMaxSize() };
        Vec2 curPos{ maxScreenSize.x / 2, maxScreenSize.y / 6 };

        // Helpful lambda for calculating the center position of the string
        auto CenterPosOfString = [](const Vec2& pos, const std::string& str) -> Vec2
        {
            return { pos.x - static_cast<int>(str.length()) / 2, pos.y };
        };

        _staticText.push_back({ _gameTitle, CenterPosOfString(curPos, _gameTitle) });

	    curPos.y += 2;

        _staticText.push_back({ _developedBy, CenterPosOfString(curPos, _developedBy) });

	    curPos.y = maxScreenSize.y / 2 - 1;

	    _staticText.push_back({ _startButtonText, CenterPosOfString(curPos, _startButtonText) });

	    curPos.y += 1;

	    _staticText.push_back({ _controlsButtonText, CenterPosOfString(curPos, _controlsButtonText) });

	    curPos.y += 1;
	    
        _staticText.push_back({ _exitButtonText, CenterPosOfString(curPos, _exitButtonText) });

        _itemPositions[MenuItem::Start] = _staticText[2].position;
        _itemPositions[MenuItem::Controls] = _staticText[3].position;
        _itemPositions[MenuItem::Exit] = _staticText[4].position;

        ResetState();
    }

    void StaticDraw() override
    {
        Renderer::EraseScreen();

        for(auto& textField : _staticText)
            Renderer::DrawText(textField.position, textField.text);
    }

    //              [PUBLIC METHODS]

    void ProcessInput(UserInput::Key key) override
    {
        int currItem { _currentItem };
        const int maxItemCount { MenuItem::MaxItemCount };

        switch(key)
        {
        case UserInput::Key::DownArrow:
            currItem++;
            currItem %= maxItemCount;

            _currentItem = static_cast<MenuItem>(currItem);
            _selectionMark.SetPosition(_itemPositions[_currentItem]);
        break;

        case UserInput::Key::UpArrow:
            currItem--;
            if(currItem < 0)
                currItem = maxItemCount - 1;
            
            _currentItem = static_cast<MenuItem>(currItem);
            _selectionMark.SetPosition(_itemPositions[_currentItem]);
        break;

        case UserInput::Key::Enter:
            switch(_currentItem)
            {
                case MenuItem::Start:
                    Screen::shouldClose = true;
                    Screen::eventListener->onEvent(ScreenChangeEvent{ ScreenType::Game });
                break;

                case MenuItem::Controls:
                    Screen::shouldClose = true;
                    Screen::eventListener->onEvent(ScreenChangeEvent{ ScreenType::Controls });
                break;

                case MenuItem::Exit:
                    Screen::shouldClose = true;
                    Screen::eventListener->onEvent(ExitEvent{});
                return;
            }
        break;

        case UserInput::Key::Escape:
            Screen::shouldClose = true;
            Screen::eventListener->onEvent(ExitEvent{});
        break;

        default:
            return;
        }
    }

    void Tick(float deltaTime) override
    {
        static float time{ 0 };
        constexpr float blinkInterval{ 0.45f };

        time += deltaTime;

        if(time > blinkInterval)
        {
            _selectionMark.SetVisibility(!_selectionMark.isVisible);
            time = 0;
        }
    }

    void ResetState() override
    {
        Screen::ResetState();
        _currentItem = MenuItem::Start;
        _selectionMark.SetPosition(_itemPositions[MenuItem::Start]);
    }

private:
    const std::string _gameTitle { "Snake!" };
    const std::string _developedBy { "developed by German" };
    const std::string _startButtonText { "start" };
    const std::string _controlsButtonText { "how to play" };
    const std::string _exitButtonText { "exit" };

    std::vector<TextField> _staticText;
    std::vector<Vec2> _itemPositions;

    MenuItem _currentItem;
    SelectionMark _selectionMark;
};