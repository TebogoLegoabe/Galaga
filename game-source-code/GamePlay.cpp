#include "GamePlay.h"

GamePlay::GamePlay()
    : gameOver(false), levelComplete(false), playerWon(false)
{
}

GamePlay::~GamePlay()
{
    // Cleanup will be handled automatically
}

void GamePlay::init()
{
    // Initialize the level
    currentLevel.initializeDefault();

    // Initialize the player at the level's start position
    player.reset(currentLevel.getPlayerStartPosition());

    // Reset game state
    gameOver = false;
    levelComplete = false;
    playerWon = false;
}

void GamePlay::handleInput()
{
    if (!gameOver && !levelComplete)
    {
        handlePlayerMovement();
    }

    // Handle menu return
    if (InputHandler::isMenuPressed())
    {
        // This will be handled by GameStateManager
    }
}

void GamePlay::update()
{
    if (!gameOver && !levelComplete)
    {
        // Update player
        player.update();

        // Update game logic
        updateGameLogic();
    }
}

void GamePlay::draw()
{
    // Clear background
    ClearBackground(BLACK);

    // Draw the level
    currentLevel.draw();

    // Draw the player
    player.draw();

    // Draw HUD
    drawHUD();

    // Draw game over or level complete messages if needed
    if (gameOver)
    {
        const char *message = playerWon ? "LEVEL COMPLETE!" : "GAME OVER";
        Color color = playerWon ? GREEN : RED;
        int textWidth = MeasureText(message, 40);
        DrawText(message, (GetScreenWidth() - textWidth) / 2,
                 GetScreenHeight() / 2 - 20, 40, color);

        const char *instruction = "Press ESC to return to menu";
        int instrWidth = MeasureText(instruction, 20);
        DrawText(instruction, (GetScreenWidth() - instrWidth) / 2,
                 GetScreenHeight() / 2 + 30, 20, WHITE);
    }
}

bool GamePlay::isGameOver() const
{
    return gameOver;
}

bool GamePlay::isLevelComplete() const
{
    return levelComplete;
}

bool GamePlay::didPlayerWin() const
{
    return playerWon;
}

void GamePlay::reset()
{
    init();
}

Player &GamePlay::getPlayer()
{
    return player;
}

Level &GamePlay::getCurrentLevel()
{
    return currentLevel;
}

void GamePlay::updateGameLogic()
{
    // TODO: Update monsters, collisions, etc. when those are implemented

    // For now, check basic win condition (no monsters implemented yet)
    // This will be expanded when we add monsters
}

void GamePlay::drawHUD()
{
    // Draw basic HUD elements
    const char *title = "DIG DUG";
    DrawText(title, 10, 10, 20, ORANGE);

    // Draw player position (for debugging)
    Vector2 gridPos = player.getGridPosition(currentLevel.getGrid());
    const char *posText = TextFormat("Grid Position: (%.0f, %.0f)", gridPos.x, gridPos.y);
    DrawText(posText, 10, 35, 15, WHITE);

    // Draw controls
    const char *controls = "Use ARROW KEYS or WASD to move and dig tunnels";
    DrawText(controls, 10, GetScreenHeight() - 45, 15, WHITE);

    const char *instruction = "Press ESC to return to menu";
    DrawText(instruction, 10, GetScreenHeight() - 25, 15, WHITE);
}

void GamePlay::handlePlayerMovement()
{
    Direction moveDirection = InputHandler::getDirectionInput();

    if (moveDirection != Direction::NONE)
    {
        player.move(moveDirection, currentLevel.getGrid());
    }
}