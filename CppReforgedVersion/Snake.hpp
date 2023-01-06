#pragma once
#include "ScreenRenderer.hpp"
#include <unordered_map>
#include <forward_list>


class Snake
{
public:
    //              [NEW TYPES]

    // Type for snake's direction handling
    enum class EDirection
    {
        Left,
        Right,
        Up,
        Down
    };


    //              [CONSTRUCTORS]

    Snake() = default;
    Snake(const Vec2& initPosition, EDirection initDirection);

    //              [SETTERS]

    void Direction(EDirection dir)
    {
        _headSymbol = _headSymbolTable.at(dir);
        _direction = EDirParser(dir);
    }


    //              [GETTERS]

    //              [UTILITY METHODS]

    void Grow();
    void Draw(Renderer::Window& win) const;
    void Move();
    

    
    
private:
    //              [NEW TYPES]

    struct Body
    {
        Vec2 direction;
        Vec2 position;

        void Move()
        {
            position += direction;
        }
    };


    //              [CONSTANTS]

    static const std::unordered_map<EDirection, char> _headSymbolTable;

    //              [PRIVATE FIELDS]

    Vec2 _headPos;
    Vec2 _direction;
    char _headSymbol;
    std::forward_list<Body> _bodyContainer;
    size_t _bodyCount;


    //              [PRIVATE METHODS]

    bool CheckGameOver();
    static Vec2 EDirParser(EDirection direction);
};