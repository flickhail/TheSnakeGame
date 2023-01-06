#pragma once

#include "UserInput.hpp"
#include "ScreenRenderer.hpp"

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

    IScreenEventObserver* eventListener;
    bool shouldClose;

    // Will be called only once at the beginning of the screen processing
    virtual void StaticDraw() = 0;

    // Processes the user input
    virtual void ProcessInput(UserInput::Key key) = 0;

    // Process the one tick of the screen
    virtual void Tick(float deltaTime) = 0;

    virtual void ResetState()
    {
        shouldClose = false;
    };

    // For better performance it is advised to call this function,
    // otherwise the Run() loop will be unnecessarily inefficient
    void WaitForUpdate(float processTime)
    {
        if(_frequencyHz < 0.1)
            return;

        if( (1.0f / _frequencyHz - processTime) <= 0)
            return;

        std::chrono::duration<float> sleepDuration{ 1.0f / _frequencyHz - processTime };
        std::this_thread::sleep_for(sleepDuration);
    }

    // Gets the frequency of the Run() loop
    int DesiredFrequency() const { return _frequencyHz; }

    // Sets the desired update frequency of the Run() loop
    // if it is 0, then the update loop
    // will be performed as fast as possible
    void DesiredFrequency(int newFreq)
    {
        _frequencyHz = newFreq;
    }

protected:
    // Construct the screen with desired frequency of the update loop
    explicit Screen(int frequency)
        : _frequencyHz{ frequency }
        , shouldClose{ false }
        , eventListener{ nullptr }
    {}

private:
    int _frequencyHz;
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