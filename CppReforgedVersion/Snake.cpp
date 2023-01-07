#include "Snake.hpp"
#include <algorithm>

const std::unordered_map<Snake::EDirection, char> Snake::_headSymbolTable
{
    { Snake::EDirection::Left, '<' },
    { Snake::EDirection::Right, '>' },
    { Snake::EDirection::Up, '^' },
    { Snake::EDirection::Down, 'v' }
};

Snake::Snake(const Vec2& initPosition, EDirection initDirection)
    : _headPos{ initPosition }
    , _direction{ EDirParser(initDirection) }
    , _headSymbol{ _headSymbolTable.at(initDirection) }
    , _bodyCount{ 0 }
{}

void Snake::Direction(EDirection dir)
{
    _headSymbol = _headSymbolTable.at(dir);
    _direction = EDirParser(dir);
}

void Snake::Grow()
{

}

// Draws the snake
void Snake::Draw(Renderer::Window& win) const
{
    Renderer::DrawChar(win, _headPos, _headSymbol);
}

// Moves the snake
void Snake::Move(Renderer::Window& win)
{
    _headPos += _direction;
    _headPos.x = std::clamp(_headPos.x, 0, win.Size().x - 1);
    _headPos.y = std::clamp(_headPos.y, 0, win.Size().y - 1);

    if(_bodyCount == 0)
        return;

    for(auto& bodyPart : _bodyContainer)
    {
        bodyPart.Move();
    }
}

// Parses the 'enum class EDirection' to 'struct Vec2'
// Returns Vec2{0,0} if error occurs
Vec2 Snake::EDirParser(EDirection direction)
{
    switch(direction)
    {
    case EDirection::Left:
        return -Vec2::Right();

    case EDirection::Right:
        return Vec2::Right();

    case EDirection::Up:
        return Vec2::Up();

    case EDirection::Down:
        return -Vec2::Up();

    default:
        return { 0, 0 };
    }
}