#pragma once

#include "Screen.hpp"
#include "ScreenRenderer.hpp"
#include "Timer.hpp"
#include "Snake.hpp"

#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>
#include <forward_list>
#include <unordered_map>

class GameScreen : public Screen
{
public:
    explicit GameScreen(int updateFrequency)
        : Screen{ updateFrequency }
    {
        // Position values was adjusted manually,
        // that is why there is some hard-coded magic numbers
        Vec2 maxPos{ Renderer::GetMaxSize() };

        {
            Vec2 gameWindowPos{ 0, 0 };
            Vec2 gameWindowSize{ maxPos.x, maxPos.y - 3 };
            _gameWindow = Renderer::Window(gameWindowSize, gameWindowPos);

            Vec2 scoreWindowPos{ 0, maxPos.y - 3 };
            Vec2 scoreWindowSize{ maxPos.x, 3 };
            _scoreWindow = Renderer::Window(scoreWindowSize, scoreWindowPos);
        }
        
        {
            Vec2 scoreTxtPos{ 2, 1 };
            Vec2 timeTxtPos{ maxPos.x - 7 - static_cast<int>(_timeText.length()), 1 };
            _scoreWinStaticText.push_back({ _scoreText, scoreTxtPos });
            _scoreWinStaticText.push_back({ _timeText, timeTxtPos });
            _scoreWinStaticText.shrink_to_fit();
            
            Vec2 timeNumberPos{ timeTxtPos.x + static_cast<int>(_timeText.length()), timeTxtPos.y };
            Vec2 scoreNumberPos{ scoreNumberPos.x + static_cast<int>(_scoreText.length()), scoreNumberPos.y };
            _timeNumber.position = timeNumberPos;
            _scoreNumber.position = scoreNumberPos;
        }

        Vec2 gameWinMaxPos{ Renderer::GetMaxSize(_gameWindow) };
        _snake = Snake(Vec2{ gameWinMaxPos.x / 2, gameWinMaxPos.y / 2 }, Snake::EDirection::Left);
    }

    void StaticDraw() override
    {
        Renderer::EraseScreen();

        for (const auto& [text, textPos] : _scoreWinStaticText)
            Renderer::DrawText(_scoreWindow, textPos, text);

        _gameWindow.DrawBorder();
        Vec2 gameWinMaxSize{ Renderer::GetMaxSize(_gameWindow) };

        // Reduce the size of the game window so there is no need in redrawing the border each frame
        _gameWindow = Renderer::Window(Vec2{gameWinMaxSize.x - 2, gameWinMaxSize.y - 2}, Vec2{1, 1});
    }

    void ProcessInput(UserInput::Key key) override
    {
        switch(key)
        {
        case UserInput::Key::LeftArrow:
            _snake.Direction(Snake::EDirection::Left);
        break;

        case UserInput::Key::RightArrow:
            _snake.Direction(Snake::EDirection::Right);
        break;

        case UserInput::Key::Escape:
            Screen::shouldClose = true;
            Screen::eventListener->onEvent(ScreenChangeEvent{ ScreenType::MainMenu });
            return;
        break;
        }
    }

    void Tick(float deltaTime) override
    {
        static float time{ 0 };
        time += deltaTime;

        UpdateTime(time);
        UpdateSnake();
    }

    void ResetState() override
    {
        Screen::ResetState();
    }

private:
    const std::string _scoreText{ "score: " };
    const std::string _timeText{ "time: " };

    std::vector<TextField> _scoreWinStaticText;
    TextField _timeNumber;
    TextField _scoreNumber;

    Renderer::Window _gameWindow;
    Renderer::Window _scoreWindow;

    Snake _snake;

    void UpdateTime(int seconds)
    {
        _timeNumber.text = std::to_string(seconds);
        Renderer::DrawText(_scoreWindow, _timeNumber.position, _timeNumber.text);
        _scoreWindow.Refresh();
    }

    void UpdateSnake()
    {
        _snake.Move();
        Renderer::EraseScreen(_gameWindow);
        _snake.Draw(_gameWindow);
        _gameWindow.Refresh();
    }
};