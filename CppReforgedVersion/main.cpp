#include "GlobalTypes.hpp"
#include "ScreenRenderer.hpp"
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

class GameLoop
{
public:
    ActiveState currentGameState;
    MenuScreen mainMenu{};
    ControlsScreen controls{};
    GameScreen game{};

    enum Screen
    {
        MainMenu,
        Controls,
        TryAgain
    };

    GameLoop() : currentGameState{ ActiveState::MenuScreen }
    {
        // Fill the 'screens'
    }

    ~GameLoop()
    {
        // Deletion of 'screens'
        for(const auto& [screenName, screenPtr] : screens)
        {
            delete screenPtr;
        }
    }

    void Run()
    {
        while(true)
        {
            switch(currentGameState)
            {
                case ActiveState::MenuScreen:
                    currentGameState = mainMenu.Run();
                break;

                case ActiveState::ControlsScreen:
                    currentGameState = controls.Run();
                break;

                case ActiveState::GameScreen:
                    currentGameState = game.Run();
                break;
                
                case ActiveState::Exit:
                return;
            }
        }
    }

private:
    std::unordered_map<std::string, int*> screens;
};

void Intermediate()
{
    constexpr Vec2 screenSize{ 50, 18 };
    ScreenRenderer::Init(screenSize);

    GameLoop gameControl{};
    gameControl.Run();

    ScreenRenderer::End();
}

int main()
{
    Intermediate();
    
    return 0;
}