#pragma once

#include <curses.h>
#include <string>
#include <vector>

// Vector class represents the X and Y axis on the screen coordinates
struct Vec2
{
public:
    int x{};
    int y{};

    constexpr Vec2() = default;
    constexpr Vec2(const Vec2& vec) = default;
    constexpr Vec2(int xValue, int yValue)
        : x(xValue)
        , y(yValue)
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

    // Short for Vec2{ 1, 0 }
    static constexpr Vec2 Right() { return { 1, 0 }; }

    // Short for Vec2{ 0, -1 }
    static constexpr Vec2 Up() { return { 0, -1 }; }
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
        : text(textToStore)
        , position(pos)
    {}
};

class UserInput
{
public:
// This enum contains the only keys that used in the game.
    enum Key
    {
        None = ERR,
        Escape = 27,
        Enter = 10,
        DownArrow = KEY_DOWN,
        UpArrow = KEY_UP,
        LeftArrow = KEY_LEFT,
        RightArrow = KEY_RIGHT,

        Undefined
    };

    static Key GetKey();
};

// class for low-level layer of terminal API
class Renderer
{
public:
    class Window
    {
    public:
        friend class Renderer;
        Window() : _winPtr { nullptr } {}

        ~Window()
        {
            delwin(_winPtr);
        }
    
        Window(const Vec2& pos, int width, int height)
        {
            _winPtr = newwin(height, width, pos.y, pos.x);
        }

        Window(const Vec2& pos, const Vec2& size)
            : Window(pos, size.x, size.y)
        {}

        Window(const Window& win) = delete;

        Window(Window&& win)
        {
            delwin(this->_winPtr);
            this->_winPtr = win._winPtr;
            win._winPtr = nullptr;
        }

        Window& operator= (const Window& win) = delete;

        Window& operator= (Window&& win)
        {
            delwin(this->_winPtr);
            this->_winPtr = win._winPtr;
            win._winPtr = nullptr;

            return *this; 
        }

        void DrawBorder()
        {
            wborder(_winPtr, 0, 0, 0, 0, 0, 0, 0, 0);
        }

        // Returns Window{} if error occurs
        Window CreateSubWindow(const Vec2& pos, const Vec2& size)
        {
            if(_winPtr == nullptr)
                return Window{};
            
            auto* subWin = subwin(_winPtr, size.y, size.x, pos.y, pos.x);

            return Window{ subWin };
        }

        void Refresh()
        {
            if(_winPtr == nullptr)
                return;
            
            touchwin(_winPtr);
            wrefresh(_winPtr);
        }

        // Returns Vec2{-1, -1} on error
        Vec2 Size() const
        {
            if(_winPtr == nullptr)
                return {-1, -1};
            
            return Renderer::GetMaxSize(*this);
        }

        void DrawText(const Vec2& position, const std::string& string)
        {
            if(_winPtr == nullptr)
                return;
            
            mvwaddstr(_winPtr, position.y, position.x, string.c_str());
        }

        void DrawChar(const Vec2& position, char symbol)
        {
            if(_winPtr == nullptr)
                return;
            
            mvwaddch(_winPtr, position.y, position.x, symbol);
        }

        void Erase()
        {
            Renderer::EraseScreen(*this);
        }

    private:
        WINDOW* _winPtr;

        Window(WINDOW* winPtr)
        {
            _winPtr = winPtr;
        }
    };

    static void Init(const Vec2& screenSize);
    static void End() { endwin(); }

    static void DrawChar(const Vec2& position, char symbol)
    { mvaddch(position.y, position.x, symbol); }

    static void DrawChar(Window& win, const Vec2& position, char symbol)
    { mvwaddch(win._winPtr, position.y, position.x, symbol); }

    // Print in the default window
    static void DrawText(const Vec2& position, const std::string& string)
    { mvaddstr(position.y, position.x, string.c_str()); }

    // Print in the custom window
    static void DrawText(Window& win, const Vec2& position, const std::string& string)
    { mvwaddstr(win._winPtr, position.y, position.x, string.c_str()); }

    static Vec2 GetMaxSize();
    static Vec2 GetMaxSize(const Window& win);
    static void Timeout(int milliseconds);
    static void Timeout(const Window& win, int milliseconds);
    static void EraseScreen() { erase(); }
    static void EraseScreen(const Window& win) { werase(win._winPtr); }
};