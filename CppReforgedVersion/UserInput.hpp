#pragma once

#include <curses.h>

class UserInput
{
public:
// This enum contains the only keys that used in the game.
// All spectrum of keys is not necessary. If you want to add new key
// it will be not so difficult. Just add a key in this enum and add 'case KEY' in
// GetKey method.
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