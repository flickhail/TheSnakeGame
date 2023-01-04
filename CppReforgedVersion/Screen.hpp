#pragma once

#include "UserInput.hpp"
#include "GlobalTypes.hpp"
#include "ScreenRenderer.hpp"

#include <chrono>
#include <thread>

// Abstract class that represents a screen that renders itself and processes
// user input.
class Screen
{
public:
    virtual ActiveState Run() = 0;

protected:
    // Construct the screen with the frequency of the update loop
    // Don't forget to call WaitForUpdate() in Run() method, otherwise
    // there is no sense in this parameter
    Screen(int frequency)
        : _frequencyHz{ frequency }
        , _sleepDuration{ 1.0f / frequency }
    {}


    // For better performance it is advised to call this function,
    // otherwise the Run() loop will be unnecessarily inefficient
    void WaitForUpdate()
    {
        std::this_thread::sleep_for(_sleepDuration);
    }

    virtual void Draw() = 0;
    virtual void ProcessInput(UserInput::Key key) = 0;

    // Gets the frequency of the Run() loop
    int Frequency() const { return _frequencyHz; }

    // Sets the frequency of the Run() loop
    void Frequency(int newFreq)
    {
        _frequencyHz = newFreq;
        _sleepDuration = std::chrono::duration<float>{ 1 / _frequencyHz };
    }

private:
    int _frequencyHz;
    std::chrono::duration<float> _sleepDuration;
};

class ScreenStorage
{
private:
    static std::vector<Screen*> screenStorage;
};