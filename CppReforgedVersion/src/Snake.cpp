#include "Snake.hpp"
#include <algorithm>

const std::unordered_map<Snake::EDirection, char> Snake::_headSymbolTable
{
    { Snake::EDirection::Left, '<' },
    { Snake::EDirection::Right, '>' },
    { Snake::EDirection::Up, '^' },
    { Snake::EDirection::Down, 'v' }
};

Snake::Snake(const Vec2& initPosition, EDirection initDirection, Renderer::Window& win)
    : m_headPos{ initPosition }
    , m_initPosition{ initPosition }
    , m_direction{ EDirParser(initDirection) }
    , m_initDirection{ initDirection }
    , m_headSymbol{ _headSymbolTable.at(initDirection) }
    , m_bodyCount{ 0 }
    , m_renderWindow{ win }
{}

void Snake::Direction(EDirection dir)
{
    switch(dir)
    {
        case EDirection::Left:
            if(m_direction == EDirParser(EDirection::Right))
                return;
        break;

        case EDirection::Right:
            if(m_direction == EDirParser(EDirection::Left))
                return;
        break;

        case EDirection::Up:
            if(m_direction == EDirParser(EDirection::Down))
                return;
        break;

        case EDirection::Down:
            if(m_direction == EDirParser(EDirection::Up))
                return;
        break;
    }

    m_headSymbol = _headSymbolTable.at(dir);
    m_direction = EDirParser(dir);
}

void Snake::Grow()
{

}

// Draws the snake
void Snake::Draw() const
{
    m_renderWindow.PrintChar(m_headPos, m_headSymbol);
}

// Moves the snake one character tile on screen
void Snake::Move()
{
    m_headPos += m_direction;

    // returns value in range [downBound, upperBound]
    auto circularClamp = [](int value, int downBound, int upperBound) -> int
    {
        if(value > upperBound)
            return downBound;
        else if (value < downBound)
            return upperBound;
        
        return value;
    };

    auto winRect{ m_renderWindow.RectField() };
    m_headPos.x = circularClamp(m_headPos.x, 0, winRect.size.x - 1);
    m_headPos.y = circularClamp(m_headPos.y, 0, winRect.size.y - 1);

    if(m_bodyCount == 0)
        return;

    for(auto& bodyPart : m_bodyContainer)
    {
        bodyPart.Move();
    }
}

void Snake::ResetState()
{
    m_headPos = m_initPosition;
    m_headSymbol = _headSymbolTable.at(m_initDirection);
    m_direction = EDirParser(m_initDirection);
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