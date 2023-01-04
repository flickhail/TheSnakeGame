#pragma once

#include <curses.h>
#include <string>
#include <vector>

class IDrawable
{
public:
    virtual void Draw() = 0;
};

class IStaticDrawable
{
public:
    virtual void StaticDraw() = 0;
};

// Vector class represents the X and Y axis on the screen coordinates
struct Vec2
{
public:
    int x{};
    int y{};

    constexpr Vec2() = default;
    constexpr Vec2(const Vec2& vec) = default;
    constexpr Vec2(int x, int y)
        : x(x)
        , y(y)
    {}

    bool operator== (const Vec2& vec) const
    {
        return { this->x == vec.x && this->y == vec.y };
    }

    Vec2 operator+= (const Vec2& vec)
    {
        return { this->x += vec.x, this->y += vec.y };
    }

    Vec2 operator+ (const Vec2& vec) const
    {
        return { this->x + vec.x, this->y + vec.y };
    }

    Vec2 operator- () const
    {
        return { -this->x, -this->y };
    }

    static constexpr Vec2 Right() { return { 1, 0 }; }
    static constexpr Vec2 Up() { return { 0, 1 }; }
};

// Text field abstraction for simple text rendering
struct TextField
{
public:
    std::string text;
    Vec2 position;

    TextField()
        : text{}
        , position{ 0, 0 }
    {}

    TextField(const std::string& textToStore, const Vec2& pos)
        : text(textToStore), position(pos)
    {}
};

// Handles the rendering on the terminal
class ScreenRenderer
{
public:
    class Window
    {
    public:
        friend class ScreenRenderer;
        Window() : _subWindow { nullptr }
        {}
    
        Window(int width, int height, const Vec2& pos)
        {
            _subWindow = subwin(stdscr, height, width, pos.y, pos.x);
        }

        Window(const Vec2& size, const Vec2& pos)
            : Window(size.x, size.y, pos)
        {}

        ~Window()
        {
            delwin(_subWindow);
        }

        Window& operator= (Window&& win)
        {
            this->_subWindow = win._subWindow;
            win._subWindow = nullptr;

            return *this; 
        }

        void DrawBorder()
        {
            wborder(_subWindow, 0, 0, 0, 0, 0, 0, 0, 0);
        }

        void Refresh()
        {
            wrefresh(_subWindow);
        }

    private:
        WINDOW* _subWindow;
    };

    static void Init(const Vec2& screenSize);
    static void End() { endwin(); }

    static void DrawChar(const Vec2& position, char symbol)
    { mvaddch(position.y, position.x, symbol); }

    static void DrawText(const Vec2& position, const std::string& string)
    { mvaddstr(position.y, position.x, string.c_str()); }

    static void DrawText(Window& win, const Vec2& position, const std::string& string)
    { mvwaddstr(win._subWindow, position.y, position.x, string.c_str()); }

    static Vec2 GetMaxSize();
    static Vec2 GetMaxSize(const Window& win);
    static void Timeout(int milliseconds);
    static void Timeout(const Window& win, int milliseconds);
    static void EraseScreen() { erase(); }

private:
    
};