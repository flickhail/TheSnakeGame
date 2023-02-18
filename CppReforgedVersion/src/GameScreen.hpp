#pragma once

#include "Screen.hpp"
#include "TerminalAPI.hpp"
#include "Timer.hpp"
#include "Snake.hpp"

#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>
#include <forward_list>
#include <unordered_map>
#include <memory>

class Food
{
public:
    Food(const Vec2& position)
    {

    }
};

class GameScreen : public Screen
{
public:
    explicit GameScreen(int updateFrequency)
        : Screen{ updateFrequency }
        , m_time{ 0.0f }
        , m_score{ 0 }
    {
        // Position values was adjusted manually,
        // that is why there is some hard-coded magic numbers
        Vec2 maxPos{ Screen::screenWindow.Size() };

        {
            Vec2 gameWindowPos{ 0, 0 };
            Vec2 gameWindowSize{ maxPos.x, maxPos.y - 3 };
            m_gameWindow = Screen::screenWindow.CreateSubWindow(gameWindowPos, gameWindowSize);

            Vec2 scoreWindowPos{ 0, maxPos.y - 3 };
            Vec2 scoreWindowSize{ maxPos.x, 3 };
            m_infoWindow = Screen::screenWindow.CreateSubWindow(scoreWindowPos, scoreWindowSize);
        }
        
        {
            Vec2 scoreTxtPos{ 2, 1 };
            Vec2 timeTxtPos{ maxPos.x - 7 - static_cast<int>(TIME_TEXT.length()), 1 };
            m_infoWinStaticText.push_back({ SCORE_TEXT, scoreTxtPos });
            m_infoWinStaticText.push_back({ TIME_TEXT, timeTxtPos });
            m_infoWinStaticText.shrink_to_fit();
            
            Vec2 timeNumberPos{ timeTxtPos.x + static_cast<int>(TIME_TEXT.length()), timeTxtPos.y };
            Vec2 scoreNumberPos{ scoreTxtPos.x + static_cast<int>(SCORE_TEXT.length()), scoreTxtPos.y };
            m_timeNumberField.position = timeNumberPos;
            m_scoreNumberField.position = scoreNumberPos;
        }

        Vec2 gameWinMaxPos{ m_gameWindow.Size() };
        m_snake = std::make_unique<Snake>(Vec2{ gameWinMaxPos.x / 2, gameWinMaxPos.y / 2 }, Snake::EDirection::Left, m_gameWindow);

        StaticDraw();
    }

    void StaticDraw()
    {
        Screen::ClearScreen();
        m_infoWindow.Erase();
        m_gameWindow.Erase();

        for (const auto& [text, textPos] : m_infoWinStaticText)
            m_infoWindow.PrintText(textPos, text);

        m_gameWindow.DrawBorder();

        Vec2 gameWinMaxSize{ m_gameWindow.Size() };

        // Reduce the size of the game window so there is no need in redrawing the border each frame
        m_gameWindow = Screen::screenWindow.CreateSubWindow(Vec2{1, 1}, Vec2{gameWinMaxSize.x - 2, gameWinMaxSize.y - 2});
    }

    void UpdateScreenBuffer() override
    {
        Renderer::Window* wins[3] = {&(Screen::screenWindow), &m_gameWindow, &m_infoWindow};
        Renderer::Window::CascadeRefresh(wins, 3);
    }

    void ProcessInput(UserInput::Key key) override
    {
        switch(key)
        {
        case UserInput::Key::LeftArrow:
            m_snake->Direction(Snake::EDirection::Left);
            m_score += 1;
        break;

        case UserInput::Key::RightArrow:
            m_snake->Direction(Snake::EDirection::Right);
        break;

        case UserInput::Key::UpArrow:
            m_snake->Direction(Snake::EDirection::Up);
        break;

        case UserInput::Key::DownArrow:
            m_snake->Direction(Snake::EDirection::Down);
        break;

        case UserInput::Key::Escape:
            Screen::shouldClose = true;
            Screen::eventListener->onEvent(ScreenChangeEvent{ ScreenType::MainMenu });
        return;
        }
    }

    void Tick(float deltaTime) override
    {
        m_time += deltaTime;

        UpdateTime(m_time);
        UpdateScore(m_score);
        UpdateSnake(deltaTime);
    }

    void ResetState() override
    {
        Screen::ResetState();
        m_gameWindow.Erase();
        m_snake->ResetState();

        m_time = 0.0f;
        m_timeNumberField.text = "    ";
        m_infoWindow.PrintText(m_timeNumberField.position, m_timeNumberField.text);

        m_score = 0;
        m_scoreNumberField.text = "    ";
        m_infoWindow.PrintText(m_scoreNumberField.position, m_scoreNumberField.text);
    }

private:
    const std::string SCORE_TEXT{ "score: " };
    const std::string TIME_TEXT{ "time: " };

    std::vector<TextField> m_infoWinStaticText;
    TextField m_timeNumberField;
    float m_time;

    TextField m_scoreNumberField;
    int m_score;

    Renderer::Window m_gameWindow;
    Renderer::Window m_infoWindow;

    std::unique_ptr<Snake> m_snake;

    void UpdateTime(int seconds)
    {
        m_timeNumberField.text = std::to_string(seconds);
        m_infoWindow.PrintText(m_timeNumberField.position, m_timeNumberField.text);
    }

    void UpdateScore(int score)
    {
        m_scoreNumberField.text = std::to_string(score);
        m_infoWindow.PrintText(m_scoreNumberField.position, m_scoreNumberField.text);
    }

    void UpdateSnake(float deltaTime)
    {
        // char's per second
        constexpr float snakeSpeed{ 10 };
        static float time{ 0 };
        time += deltaTime;

        if(time >= 1.0f / snakeSpeed)
        {
            //m_gameWindow.Erase();
            m_snake->Move();
            m_snake->Draw();
            time = 0;
        }
    }
};