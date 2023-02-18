#include "TerminalAPI.hpp"
#include "Screen.hpp"
#include "MenuScreen.hpp"
#include "ControlsScreen.hpp"
#include "GameScreen.hpp"

#include <string>
#include <vector>
#include <ratio>
#include <unordered_map>
#include <thread>

// [TMP]
#include <iostream>

class GameDriver : public IScreenEventObserver
{
private:
    bool m_shouldStop;
    std::unordered_map<Screen::ScreenType, Screen*> m_screens;
    Screen::ScreenType m_currentScreen;
public:

    GameDriver()
    : m_currentScreen{ Screen::ScreenType::MainMenu }
    , m_shouldStop{ false }
    {
        m_screens[Screen::ScreenType::MainMenu] = new MenuScreen{ 60 };
        m_screens[Screen::ScreenType::Controls] = new ControlsScreen{ 60 };
        m_screens[Screen::ScreenType::Game] = new GameScreen{ 60 };

        // [NOTE] Observer pattern
        for(auto& [screenEnum, screenPtr] : m_screens)
            screenPtr->StartListen(this);
    }

    ~GameDriver() override
    {
        // Deletion of 'screens'
        for(auto& [screenEnum, screenPtr] : m_screens)
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
        while(!m_shouldStop)
        {
            auto screen = m_screens[m_currentScreen];
            screen->ResetState();

            // screen processing loop
            while(!screen->ShouldClose())
            {
                deltaTime = timer.Elapsed() - lastFrame;
                lastFrame = timer.Elapsed();

                float startProcessingTime{ timer.Elapsed() };

                screen->ProcessInput(UserInput::GetKey());
                screen->Tick(deltaTime);
                screen->UpdateScreenBuffer();
                
                float procTime{ timer.Elapsed() - startProcessingTime };

                screen->WaitForUpdate(procTime);
            }
        }
    }

    // callbacks for 'Screen' objects

    void onEvent(const ExitEvent& event) override
    {
        m_shouldStop = true;
    }

    void onEvent(const ScreenChangeEvent& event) override
    {
        m_currentScreen = event.screenChangedTo;
    }
};

void Intermediate()
{
    constexpr Vec2 screenSize{ 50, 18 };
    Renderer::Init(screenSize);

    GameDriver().Run();

    Renderer::End();
}

void Wait()
{
    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(2s);
}

int main()
{
    //Intermediate();
#ifdef SNK_TEST
    std::cout << "SNK_TEST is DEFINED\n";
#endif

#ifdef SNK_RELEASE
    std::cout << "SNK_RELEASE is DEFINED\n";
#endif
    return 0;
}
