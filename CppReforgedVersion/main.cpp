#include "TerminalAPI.hpp"
#include "Screen.hpp"
#include "MenuScreen.hpp"
#include "ControlsScreen.hpp"
#include "GameScreen.hpp"

#include <string>
#include <vector>
#include <ratio>
#include <unordered_map>

// [TMP]
#include <iostream>

const char* gameOverText = "Game over";
const char* tryAgainText = "try again";
const char* returnToMainMenuText = "main menu";
const char* scoreText = "score: ";
const char* timeText = "time: ";

const char goal_symb = 'x';
const char body_symbol = 'x';
const char left_arrow = '<';
const char right_arrow = '>';
const char up_arrow = '^';
const char down_arrow = 'v';

class GameLoop : public IScreenEventObserver
{
public:
    bool shouldStop;

    std::unordered_map<Screen::ScreenType, Screen*> screens;
    Screen::ScreenType currentScreen;

    GameLoop()
    : currentScreen{ Screen::ScreenType::MainMenu }
    , shouldStop{ false }
    {
        screens[Screen::ScreenType::MainMenu] = new MenuScreen{ 30 };
        screens[Screen::ScreenType::Controls] = new ControlsScreen{ 30 };
        screens[Screen::ScreenType::Game] = new GameScreen{ 60 };

        // [NOTE]
        // Observer pattern
        for(const auto& [screenEnum, screenPtr] : screens)
            screenPtr->eventListener = this;
    }

    ~GameLoop() override
    {
        // Deletion of 'screens'
        for(auto& [screenEnum, screenPtr] : screens)
        {
            delete screenPtr;
            screenPtr = nullptr;
        }
    }

    void Run()
    {
        Timer<float> timer;
        float lastFrame = timer.Elapsed();
        float deltaTime = 0;

        // Main game loop
        while(!shouldStop)
        {
            auto screen = screens[currentScreen];
            screen->ResetState();

            // screen processing loop
            while(!screen->shouldClose)
            {
                deltaTime = timer.Elapsed() - lastFrame;
                lastFrame = timer.Elapsed();

                float startProcessingTime{ timer.Elapsed() };
                screen->ProcessInput(UserInput::GetKey());
                screen->Tick(deltaTime);
                screen->UpdateBuffer();
                float procTime{ timer.Elapsed() - startProcessingTime };

                screen->WaitForUpdate(procTime);
            }
        }
    }

    // callbacks for 'Screen' objects

    void onEvent(const ExitEvent& event) override
    {
        shouldStop = true;
    }

    void onEvent(const ScreenChangeEvent& event) override
    {
        currentScreen = event.screenChangedTo;
    }
};

void Intermediate()
{
    constexpr Vec2 screenSize{ 50, 18 };
    Renderer::Init(screenSize);

    GameLoop* gameLoop = new GameLoop{};
    gameLoop->Run();
    delete gameLoop;
    gameLoop = nullptr;

    Renderer::End();
}

void Wait()
{
    while(UserInput::GetKey() == UserInput::Key::None);
}

int main()
{
    Intermediate();

    return 0;
}