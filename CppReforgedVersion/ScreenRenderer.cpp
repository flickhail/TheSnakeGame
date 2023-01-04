#include "ScreenRenderer.hpp"



void ScreenRenderer::Init(const Vec2& screenSize)
{
    initscr();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	nodelay(stdscr, TRUE);
	resize_term(screenSize.y, screenSize.x);
}

Vec2 ScreenRenderer::GetMaxSize()
{
	int maxX;
	int maxY;

	getmaxyx(stdscr, maxY, maxX);

	return { maxX, maxY };
}

Vec2 ScreenRenderer::GetMaxSize(const ScreenRenderer::Window& win)
{
	int maxX;
	int maxY;

	getmaxyx(win._subWindow, maxY, maxX);
	return { maxX, maxY };
}

void ScreenRenderer::Timeout(int milliseconds)
{
	timeout(milliseconds);
}

void ScreenRenderer::Timeout(const ScreenRenderer::Window& win, int milliseconds)
{
	wtimeout(win._subWindow, milliseconds);
}