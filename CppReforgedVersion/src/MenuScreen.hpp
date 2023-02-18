#pragma once

#include "Screen.hpp"
#include "TerminalAPI.hpp"

#include <vector>

class SelectionMark
    {
    public:
        bool isVisible{ true };

        SelectionMark() = delete;
        SelectionMark(Vec2 pos, Renderer::Window& win)
        : isVisible{ true }
        , window{ win }
        {
            currentPos = { pos.x - markIndent, pos.y };
            window.PrintChar(currentPos, markSymbol);

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
                window.PrintChar(lastPos, ' ');
                window.PrintChar(currentPos, markSymbol);
            }
            else
            {
                currentPos = { newPos.x - markIndent, newPos.y };
            }
        }

        void Hide()
        {
            window.PrintChar(currentPos, ' ');
            isVisible = false;
        }

        void Show()
        {
            window.PrintChar(currentPos, markSymbol);
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

    private:
        const char markSymbol = '>';
        const int markIndent = 2;
        Vec2 currentPos{};
        Vec2 lastPos{};

        // Window to draw in
        Renderer::Window& window;
    };

class MenuScreen : public Screen
{
public:
    enum MenuItem
    {
        Start,
        Controls,
        Exit,

        MaxItemCount
    };

    explicit MenuScreen(int updateFrequency)
        : Screen{ updateFrequency }
        , m_itemPositions{ MenuItem::MaxItemCount }
        , m_selectionMark{ Vec2{0, 0}, Screen::screenWindow }
    {
        // The whole point of this constructor is to calculate interface element's positions only once and store them in variables

	    Vec2 maxScreenSize{ Renderer::GetMaxSize() };
        Vec2 curPos{ maxScreenSize.x / 2, maxScreenSize.y / 6 };

        // Helpful lambda for calculating the center position of the string
        auto StrCentr = [](const Vec2& pos, const std::string& str) -> Vec2
        {
            return { pos.x - static_cast<int>(str.length()) / 2, pos.y };
        };

        m_staticText.push_back({ GAME_TITLE, StrCentr(curPos, GAME_TITLE) });

	    curPos.y += 2;

        m_staticText.push_back({ DEVELOPED_BY, StrCentr(curPos, DEVELOPED_BY) });

	    curPos.y = maxScreenSize.y / 2 - 1;

	    m_staticText.push_back({ START_BUTTON_TEXT, StrCentr(curPos, START_BUTTON_TEXT) });

	    curPos.y += 1;

	    m_staticText.push_back({ CONTROLS_BUTTON_TEXT, StrCentr(curPos, CONTROLS_BUTTON_TEXT) });

	    curPos.y += 1;
	    
        m_staticText.push_back({ EXIT_BUTTON_TEXT, StrCentr(curPos, EXIT_BUTTON_TEXT) });
        m_staticText.shrink_to_fit();

        m_itemPositions[MenuItem::Start] = m_staticText[2].position;
        m_itemPositions[MenuItem::Controls] = m_staticText[3].position;
        m_itemPositions[MenuItem::Exit] = m_staticText[4].position;

        StaticDraw();
        ResetState();
    }

    // Fills the window buffer with static text
    void StaticDraw()
    {
        Screen::ClearScreen();

        for(auto& textField : m_staticText)
            Screen::screenWindow.PrintText(textField.position, textField.text);
    }

    void UpdateScreenBuffer() override
    {
        Screen::screenWindow.Refresh();
    }

    void ProcessInput(UserInput::Key key) override
    {
        int currItem { m_currentItem };
        const int maxItemCount { MenuItem::MaxItemCount };

        switch(key)
        {
        case UserInput::Key::DownArrow:
            currItem++;
            currItem %= maxItemCount;

            m_currentItem = static_cast<MenuItem>(currItem);
            m_selectionMark.SetPosition(m_itemPositions[m_currentItem]);
        break;

        case UserInput::Key::UpArrow:
            currItem--;
            if(currItem < 0)
                currItem = maxItemCount - 1;
            
            m_currentItem = static_cast<MenuItem>(currItem);
            m_selectionMark.SetPosition(m_itemPositions[m_currentItem]);
        break;

        case UserInput::Key::Enter:
            switch(m_currentItem)
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
            m_selectionMark.SetVisibility(!m_selectionMark.isVisible);
            time = 0;
        }
    }

    void ResetState() override
    {
        Screen::ResetState();
        m_currentItem = MenuItem::Start;
        m_selectionMark.SetPosition(m_itemPositions[MenuItem::Start]);
    }

private:
    const std::string GAME_TITLE { "Snake!" };
    const std::string DEVELOPED_BY { "developed by German" };
    const std::string START_BUTTON_TEXT { "start" };
    const std::string CONTROLS_BUTTON_TEXT { "how to play" };
    const std::string EXIT_BUTTON_TEXT { "exit" };

    std::vector<TextField> m_staticText;
    std::vector<Vec2> m_itemPositions;

    MenuItem m_currentItem;
    SelectionMark m_selectionMark;
};