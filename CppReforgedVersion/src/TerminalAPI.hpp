#pragma once

#include <curses.h>
#include <string>
#include <vector>

// Vector class represents the X and Y axis on the screen coordinates
struct Vec2
{
    int x{};
    int y{};

    constexpr Vec2() = default;
    constexpr Vec2(const Vec2& vec) = default;
    constexpr Vec2(int xValue, int yValue)
        : x(xValue)
        , y(yValue)
    {}

    bool operator== (const Vec2& vec) const;
    Vec2 operator+= (const Vec2& vec);
    Vec2 operator+ (const Vec2& vec) const;
    Vec2 operator- () const;

    // Short for Vec2{ 1, 0 }
    static constexpr Vec2 Right() { return { 1, 0 }; }

    // Short for Vec2{ 0, -1 }
    static constexpr Vec2 Up() { return { 0, -1 }; }
};

// Represents a rectangle with origin point and the size
struct Rect
{
    Vec2 origin{};
    Vec2 size{};

    constexpr Rect() = default;
    constexpr Rect(const Vec2& a_origin, const Vec2& a_size)
    : origin{ a_origin }
    , size{ a_size }
    {}
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

// class that encapsulates low-level layer of terminal API
class Renderer
{
public:
    class Window
    {
    public:
        Window();
        Window(const Vec2& pos, const Vec2& size);
        Window(const Window& win) = delete;
        Window(Window&& win);

        ~Window();

        Window& operator=(const Window& win) = delete;
        Window& operator=(Window&& win);

        // Returns Vec2() on error
        Vec2 Size() const;

        // Returns Rect() on error
        Rect RectField() const;

        // Returns Window{} if error occurs
        Window CreateSubWindow(const Vec2& pos, const Vec2& size);

        // More performant way to update several windows in one call
        static void CascadeRefresh(Window** windows, size_t size);

        // Update the visual output on the terminal
        void Refresh();

        // Draws a border using the size of the window
        void DrawBorder();
        void PrintText(const Vec2& position, const std::string& string);
        void PrintChar(const Vec2& position, char symbol);

        // Clears the window
        void Erase();

    private:
        friend class Renderer;
        friend class UserInput;

        WINDOW* m_winPtr;
        Rect m_winRect;
        bool m_isChanged{ true };

        Window(WINDOW* winPtr, const Rect& winRect);
    };

    static void Init(const Vec2& screenSize);
    static void End() { endwin(); }

    static void PrintChar(const Vec2& position, char symbol)
    { mvaddch(position.y, position.x, symbol); }

    static void PrintChar(Window& win, const Vec2& position, char symbol)
    { mvwaddch(win.m_winPtr, position.y, position.x, symbol); }

    // Print in the default window
    static void PrintText(const Vec2& position, const std::string& string)
    { mvaddstr(position.y, position.x, string.c_str()); }

    // Print in the custom window
    static void PrintText(Window& win, const Vec2& position, const std::string& string)
    { mvwaddstr(win.m_winPtr, position.y, position.x, string.c_str()); }

    static Vec2 GetMaxSize();
    static Vec2 GetMaxSize(const Window& win);
    static void Timeout(int milliseconds);
    static void Timeout(const Window& win, int milliseconds);
    static void EraseScreen() { erase(); }
    static void EraseScreen(const Window& win) { werase(win.m_winPtr); }
};