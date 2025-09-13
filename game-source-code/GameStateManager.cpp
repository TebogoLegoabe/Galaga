#include "GameStateManager.h"
#include "Menu.h"
#include "GamePlay.h"

GameStateManager::GameStateManager()
    : currentState(GameState::MENU), exitRequested(false)
{
}

GameStateManager::~GameStateManager()
{
    // Smart pointers will handle cleanup automatically
}

void GameStateManager::init()
{
    // Initialize menu state
    menuState = std::make_unique<Menu>();

    // GamePlay will be initialized when needed
    currentState = GameState::MENU;
}

void GameStateManager::update()
{
    switch (currentState)
    {
    case GameState::MENU:
        if (menuState)
        {
            menuState->handleInput();
            menuState->update();

            // Check for state transitions
            if (menuState->shouldStartGame())
            {
                switchState(GameState::PLAYING);
            }
            else if (menuState->shouldExitGame())
            {
                exitRequested = true;
            }
        }
        break;

    case GameState::PLAYING:
        if (gamePlayState)
        {
            gamePlayState->handleInput();
            gamePlayState->update();

            // Check if we should return to menu
            if (IsKeyPressed(KEY_ESCAPE))
            {
                switchState(GameState::MENU);
            }

            // Check for game over
            if (gamePlayState->isGameOver())
            {
                menuState->setGameOver(gamePlayState->didPlayerWin());
                switchState(GameState::MENU);
            }
        }
        break;

    case GameState::PAUSED:
        // TODO: Implement pause functionality
        break;

    case GameState::GAME_OVER:
        // Handled by menu system
        break;

    case GameState::LEVEL_COMPLETE:
        // Handled by menu system
        break;
    }
}

void GameStateManager::draw()
{
    switch (currentState)
    {
    case GameState::MENU:
    case GameState::GAME_OVER:
    case GameState::LEVEL_COMPLETE:
        if (menuState)
        {
            menuState->draw();
        }
        break;

    case GameState::PLAYING:
    {
        if (gamePlayState)
        {
            gamePlayState->draw();
        }
        break;
    }

    case GameState::PAUSED:
        // TODO: Draw pause overlay
        if (gamePlayState)
        {
            gamePlayState->draw();
            // Add pause overlay here
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                          Color{0, 0, 0, 128});
            const char *pauseText = "PAUSED";
            int textWidth = MeasureText(pauseText, 40);
            DrawText(pauseText, (GetScreenWidth() - textWidth) / 2,
                     GetScreenHeight() / 2, 40, WHITE);
        }
        break;
    }
}

void GameStateManager::switchState(GameState newState)
{
    if (currentState == newState)
        return;

    // Handle state exit logic
    switch (currentState)
    {
    case GameState::MENU:
        // Nothing special needed
        break;
    case GameState::PLAYING:
        // Could save game state if needed
        break;
    default:
        break;
    }

    // Update current state
    currentState = newState;

    // Handle state entry logic
    switch (newState)
    {
    case GameState::MENU:
        if (menuState)
        {
            menuState->reset();
        }
        break;
    case GameState::PLAYING:
        if (!gamePlayState)
        {
            gamePlayState = std::make_unique<GamePlay>();
            gamePlayState->init();
        }
        else
        {
            gamePlayState->reset();
        }
        break;
    default:
        break;
    }
}

GameState GameStateManager::getCurrentState() const
{
    return currentState;
}

bool GameStateManager::shouldExit() const
{
    return exitRequested;
}