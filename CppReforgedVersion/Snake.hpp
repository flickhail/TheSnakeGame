#pragma once

#include "TerminalAPI.hpp"

#include <unordered_map>
#include <forward_list>


class Snake
{
public:
    // Type for snake's direction handling
    enum class EDirection
    {
        Left,
        Right,
        Up,
        Down
    };

    Snake() = default;
    Snake(const Vec2& initPosition, EDirection initDirection);

    void Direction(EDirection dir);

    void Grow();
    void Draw(Renderer::Window& win) const;
    void Move(Renderer::Window& win);
    void ResetState();
    
private:
    struct Body
    {
        Vec2 direction;
        Vec2 position;

        void Move()
        {
            position += direction;
        }
    };

    static const std::unordered_map<EDirection, char> _headSymbolTable;

    Vec2 _initPosition;
    Vec2 _headPos;
    EDirection _initDirection;
    Vec2 _direction;
    char _headSymbol;
    std::forward_list<Body> _bodyContainer;
    size_t _bodyCount;

    bool CheckGameOver();
    static Vec2 EDirParser(EDirection direction);
};