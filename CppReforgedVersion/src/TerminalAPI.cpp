#include "TerminalAPI.hpp"

bool Vec2::operator==(const Vec2& vec) const
{
    return { this->x == vec.x && this->y == vec.y };
}

Vec2 Vec2::operator+=(const Vec2& vec)
{
    return { this->x += vec.x, this->y += vec.y };
}

Vec2 Vec2::operator+(const Vec2& vec) const
{
    return { this->x + vec.x, this->y + vec.y };
}

Vec2 Vec2::operator-() const
{
    return { -this->x, -this->y };
}



UserInput::Key UserInput::GetKey()
{
    int key = getch();

    switch(key)
    {
    case UserInput::Key::Enter:
    case UserInput::Key::Escape:
    case UserInput::Key::DownArrow:
    case UserInput::Key::UpArrow:
    case UserInput::Key::LeftArrow:
    case UserInput::Key::RightArrow:
    case UserInput::Key::None:
        return static_cast<UserInput::Key>(key);

    default:
        return Key::Undefined;
    }
}



Renderer::Window::Window()
    : m_winPtr{ nullptr }
    , m_winRect{}
{}

Renderer::Window::Window(const Vec2& pos, const Vec2& size)
    : m_winPtr(newwin(size.y, size.x, pos.y, pos.x))
    , m_winRect{ pos, size }
{}

Renderer::Window::Window(Window&& win)
    : m_winRect{ win.m_winRect }
{
    delwin(this->m_winPtr);
    this->m_winPtr = win.m_winPtr;
    win.m_winPtr = nullptr;
}

Renderer::Window::Window(WINDOW* winPtr, const Rect& winRect)
    : m_winPtr{ winPtr }
    , m_winRect{ winRect }
{}

Renderer::Window::~Window()
{
    delwin(m_winPtr);
    m_winPtr = nullptr;
}

Renderer::Window& Renderer::Window::operator=(Window&& win)
{
    delwin(this->m_winPtr);
    this->m_winPtr = win.m_winPtr;
    win.m_winPtr = nullptr;

    this->m_winRect = win.m_winRect;

    return *this; 
}

Renderer::Window Renderer::Window::CreateSubWindow(const Vec2& pos, const Vec2& size)
{
    if(m_winPtr == nullptr)
        return Window{};
            
    WINDOW* subWin = subwin(m_winPtr, size.y, size.x, pos.y, pos.x);

    return Window{ subWin, Rect{ pos, size } };
}

void Renderer::Window::CascadeRefresh(Window** windows, size_t size)
{
    if(windows == nullptr || size == 0)
        return;

    for(size_t i{0}; i < size; ++i)
    {
        if(!windows[i]->m_isChanged)
            touchwin(windows[i]->m_winPtr);
        
        wnoutrefresh(windows[i]->m_winPtr);
        windows[i]->m_isChanged = false;
    }

    doupdate();
}

void Renderer::Window::Refresh()
{
    // attempt to optimize draw calls, because operations with m_winPtr
    // is expensive
    if(!m_isChanged)
        touchwin(m_winPtr);
    
    wrefresh(m_winPtr);
    m_isChanged = false;
}

Vec2 Renderer::Window::Size() const
{
    return m_winRect.size;
}

Rect Renderer::Window::RectField() const
{
    return m_winRect;
}

void Renderer::Window::DrawBorder()
{
    wborder(m_winPtr, 0, 0, 0, 0, 0, 0, 0, 0);
    m_isChanged = true;
}

void Renderer::Window::PrintText(const Vec2& position, const std::string& string)
{
    mvwaddstr(m_winPtr, position.y, position.x, string.c_str());
    m_isChanged = true;
}

void Renderer::Window::PrintChar(const Vec2& position, char symbol)
{
    mvwaddch(m_winPtr, position.y, position.x, symbol);
    m_isChanged = true;
}

void Renderer::Window::Erase()
{
    if(m_winPtr == nullptr)
        return;
    
    Renderer::EraseScreen(*this);
    m_isChanged = true;
}



void Renderer::Init(const Vec2& screenSize)
{
    initscr();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	nodelay(stdscr, TRUE);
	resize_term(screenSize.y, screenSize.x);
}

Vec2 Renderer::GetMaxSize()
{
	int maxX;
	int maxY;

	getmaxyx(stdscr, maxY, maxX);

	return { maxX, maxY };
}

Vec2 Renderer::GetMaxSize(const Renderer::Window& win)
{
	int maxX;
	int maxY;

	getmaxyx(win.m_winPtr, maxY, maxX);
	return { maxX, maxY };
}

void Renderer::Timeout(int milliseconds)
{
	timeout(milliseconds);
}

void Renderer::Timeout(const Renderer::Window& win, int milliseconds)
{
	wtimeout(win.m_winPtr, milliseconds);
}