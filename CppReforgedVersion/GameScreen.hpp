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
    GameScreen()
        : Screen{ 60 }
    {
        // Position values was adjusted manually,
        // that is why there is some hard-coded magic numbers
        Vec2 maxPos{ ScreenRenderer::GetMaxSize() };

        {
            Vec2 gameWindowPos{ 0, 0 };
            Vec2 gameWindowSize{ maxPos.x, maxPos.y - 3 };
            _gameWindow = ScreenRenderer::Window(gameWindowSize, gameWindowPos);

            Vec2 scoreWindowPos{ 0, maxPos.y - 3 };
            Vec2 scoreWindowSize{ maxPos.x, 3 };
            _scoreWindow = ScreenRenderer::Window(scoreWindowSize, scoreWindowPos);
        }
        
        {
            Vec2 scoreTxtPos{ 2, 1 };
            _scoreWindowTextToRender.push_back({ _scoreText, scoreTxtPos });

            Vec2 timeTxtPos{ maxPos.x - 7 - static_cast<int>(_timeText.length()), 1 };
            Vec2 timeNumberPos{ timeTxtPos.x + static_cast<int>(_timeText.length()), timeTxtPos.y };
            _scoreWindowTextToRender.push_back({ _timeText, timeTxtPos });
            _timeNumber.position = timeNumberPos;

            _scoreWindowTextToRender.shrink_to_fit();
        }

        Vec2 gameWinMaxPos{ ScreenRenderer::GetMaxSize(_gameWindow) };
        _snake = Snake(Vec2{ gameWinMaxPos.x / 2, gameWinMaxPos.y / 2 }, Snake::EDirection::Left);
    }

    ActiveState Run() override
    {
        Draw();
        auto key{ UserInput::Undefined };

        Timer<int> timer{};

        while ((key = UserInput::GetKey()) != UserInput::Key::Escape)
        {
            ProcessInput(key);


            UpdateTime(timer.Elapsed());
            _snake.Draw();

            Screen::WaitForUpdate();
        }

        return ActiveState::MenuScreen;
    }

private:
    ScreenRenderer::Window _gameWindow;
    ScreenRenderer::Window _scoreWindow;

    TextField _timeNumber;
    TextField _scoreNumber;

    const std::string _scoreText{ "score: " };
    const std::string _timeText{ "time: " };

    std::vector<TextField> _gameWindowTextToRender;
    std::vector<TextField> _scoreWindowTextToRender;

    Snake _snake;

    void Draw() override
    {
        ScreenRenderer::EraseScreen();

        // Score window drawing [STATIC]
        for (const auto& [text, textPos] : _scoreWindowTextToRender)
            ScreenRenderer::DrawText(_scoreWindow, textPos, text);

        // Game window drawing [STATIC]
        _gameWindow.DrawBorder();
    }

    void ProcessInput(UserInput::Key key) override
    {
        switch(key)
        {
        case UserInput::Key::None:
            _snake.Move();
            break;

        case UserInput::Key::LeftArrow:
            _snake.Direction(Snake::EDirection::Left);
            _snake.Move();
            break;

        case UserInput::Key::RightArrow:
            _snake.Direction(Snake::EDirection::Right);
            _snake.Move();
            break;
        }
    }

    void UpdateTime(int seconds)
    {
        _timeNumber.text = std::to_string(seconds);
        ScreenRenderer::DrawText(_scoreWindow, _timeNumber.position, _timeNumber.text);
        _scoreWindow.Refresh();
    }
};