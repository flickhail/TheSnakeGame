#include "UserInput.hpp"

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