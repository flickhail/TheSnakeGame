#include "ScreenRenderer.hpp"

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

	getmaxyx(win._subWindow, maxY, maxX);
	return { maxX, maxY };
}

void Renderer::Timeout(int milliseconds)
{
	timeout(milliseconds);
}

void Renderer::Timeout(const Renderer::Window& win, int milliseconds)
{
	wtimeout(win._subWindow, milliseconds);
}