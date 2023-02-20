#pragma once

#include "TerminalAPI.hpp"

#include <chrono>
#include <thread>

class IScreenEventObserver;

// Abstract class that represents a screen that renders itself and processes
// user input.
class Screen
{
public:
    enum ScreenType
    {
        MainMenu,
        Controls,
        Game,
        TryAgain
    };

    // Processes the user input
    virtual void ProcessInput(UserInput::Key key) = 0;

    // Process the one tick of the screen
    virtual void Tick(float deltaTime) = 0;

    // Updates the screen's buffer
    virtual void UpdateScreenBuffer() = 0;

    virtual void ResetState()
    {
        shouldClose = false;
    }

    // Sleep on current thread so that the update frequency was satisfied
    void WaitForUpdate(float processTime)
    {
        if(m_frequencyHz < 0.1)
            return;

        if( (1.0f / m_frequencyHz - processTime) <= 0)
            return;

        std::chrono::duration<float> sleepDuration{ 1.0f / m_frequencyHz - processTime };
        std::this_thread::sleep_for(sleepDuration);
    }

    void AddEventObserver(IScreenEventObserver* observer)
    {
        eventListener = observer;
    }

    bool ShouldClose() const
    {
        return shouldClose;
    }

protected:
    Renderer::Window screenWindow;
    IScreenEventObserver* eventListener;
    bool shouldClose;

    // Construct the screen with desired frequency of the update loop
    explicit Screen(int frequency)
        : m_frequencyHz{ frequency }
        , shouldClose{ false }
        , eventListener{ nullptr }
        , screenWindow{ Vec2{ 0, 0 }, Renderer::GetMaxSize() }
    {}

    void ClearScreen()
    {
        Renderer::EraseScreen(screenWindow);
    }

private:
    int m_frequencyHz;
};

class Event
{
public:
    enum class EventType
    {
        ScreenChanged,
        ApplicationExit
    };

    EventType type;

    Event(EventType newType)
    : type{ newType }
    {}
};

class ScreenChangeEvent : public Event
{
public:
    Screen::ScreenType screenChangedTo;

    explicit ScreenChangeEvent(Screen::ScreenType newScreen)
    : Event{ EventType::ScreenChanged }
    , screenChangedTo{ newScreen }
    {}
};

class ExitEvent : public Event
{
public:
    ExitEvent()
    : Event{ EventType::ApplicationExit }
    {}
};

class IScreenEventObserver
{
public:
    virtual ~IScreenEventObserver() {};
    virtual void onEvent(const ScreenChangeEvent& event) = 0;
    virtual void onEvent(const ExitEvent& event) = 0;
};