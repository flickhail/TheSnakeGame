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
    Snake(const Vec2& initPosition, EDirection initDirection, Renderer::Window& win);
    ~Snake() {};

    void Direction(EDirection dir);

    void Grow();
    void Draw() const;
    void Move();
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

    Vec2 m_initPosition;
    Vec2 m_headPos;
    EDirection m_initDirection;
    Vec2 m_direction;
    char m_headSymbol;
    std::forward_list<Body> m_bodyContainer;
    size_t m_bodyCount;
    Renderer::Window& m_renderWindow;

    bool CheckGameOver();
    static Vec2 EDirParser(EDirection direction);
};