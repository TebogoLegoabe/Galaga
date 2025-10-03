#include "GamePlay.h"

const float GamePlay::DISEMBODIED_COOLDOWN_TIME = 3.0f; // 3 seconds between disembodied transitions

GamePlay::GamePlay()
    : gameOver(false), levelComplete(false), playerWon(false), disembodiedCooldown(0.0f)
{
}

GamePlay::~GamePlay()
{
    // Cleanup will be handled automatically by smart pointers and members
}

void GamePlay::init()
{
    // Initialize the level
    currentLevel.initializeDefault();

    // Initialize the player at the level's start position
    player.reset(currentLevel.getPlayerStartPosition());

    // Reset lives only when starting a completely new game
    player.resetLives();

    // Initialize monsters
    monsterManager.initialize(currentLevel, currentLevel.getPlayerStartPosition());

    // Reset game state
    gameOver = false;
    levelComplete = false;
    playerWon = false;
    disembodiedCooldown = 0.0f;
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
        // Update disembodied cooldown timer
        if (disembodiedCooldown > 0.0f)
        {
            disembodiedCooldown -= GetFrameTime();
            if (disembodiedCooldown < 0.0f)
            {
                disembodiedCooldown = 0.0f;
            }
        }

        // Update player
        player.update();

        // Update monsters
        monsterManager.update(player, currentLevel.getGrid(), canMonsterBecomeDisembodied(),
                              [this]()
                              { notifyMonsterBecameDisembodied(); });

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

    // Draw monsters
    monsterManager.draw();

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

std::vector<std::unique_ptr<Monster>> &GamePlay::getMonsters()
{
    return monsterManager.getMonsters();
}

void GamePlay::updateGameLogic()
{
    // Update player harpoon and check for collisions with monsters
    Harpoon &harpoon = player.getHarpoon();
    if (harpoon.isHarpoonActive())
    {
        // Check if harpoon should be destroyed (hit wall, etc.)
        if (harpoon.shouldDestroy(currentLevel.getGrid()))
        {
            harpoon.deactivate();
        }
        else
        {
            // Check harpoon-monster collisions
            CollisionHandler::checkHarpoonMonsterCollisions(player, monsterManager,
                                                            currentLevel.getGrid());
        }
    }

    if (CollisionHandler::checkFirePlayerCollision(player, monsterManager))
    {
        // Player hit by fire - lose a life
        bool stillAlive = player.loseLife();

        if (!stillAlive)
        {
            gameOver = true;
            playerWon = false;
        }
        else
        {
            respawnPlayer();
        }
        return; // Don't check other collisions this frame
    }

    // Check for collisions between player and monsters
    if (CollisionHandler::checkPlayerMonsterCollision(player, monsterManager))
    {
        // Player collided with monster - lose a life
        bool stillAlive = player.loseLife(); // Decrements lives by 1

        if (!stillAlive) // This means lives == 0
        {
            // No lives left - game over
            gameOver = true;
            playerWon = false;
        }
        else
        {
            // Still has lives - respawn player at start position
            respawnPlayer();
        }
    }

    // Check if all monsters are dead (win condition)
    if (monsterManager.areAllMonstersDead())
    {
        gameOver = true;
        levelComplete = true;
        playerWon = true;
    }
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

    // Draw monster count
    int aliveMonsters = 0;
    auto &monsters = monsterManager.getMonsters();
    for (const auto &monster : monsters)
    {
        if (monster->isActive() && !monster->isDead())
            aliveMonsters++;
    }

    const char *monsterText = TextFormat("Monsters Remaining: %d", aliveMonsters);
    DrawText(monsterText, 10, 55, 15, WHITE);

    // Draw lives remaining
    const char *livesText = "Lives:";
    DrawText(livesText, 10, 75, 15, WHITE);

    // Draw life icons (small blue circles representing Dig Dug)
    int livesStartX = 60;
    int livesY = 78;
    for (int i = 0; i < player.getLives(); i++)
    {
        DrawCircleV({static_cast<float>(livesStartX + i * 25), static_cast<float>(livesY)}, 8, BLUE);
        DrawCircleV({static_cast<float>(livesStartX + i * 25), static_cast<float>(livesY)}, 3, WHITE); // Direction indicator
    }
}

void GamePlay::handlePlayerMovement()
{
    Direction moveDirection = InputHandler::getDirectionInput();

    if (moveDirection != Direction::NONE)
    {
        player.move(moveDirection, currentLevel.getGrid());
    }

    // Handle shooting
    if (InputHandler::isActionPressed())
    {
        player.shoot();
    }
}

bool GamePlay::canMonsterBecomeDisembodied() const
{
    // Check if cooldown allows a new disembodied monster
    if (disembodiedCooldown > 0.0f)
        return false;

    // Check if there's already a disembodied monster
    auto &monsters = monsterManager.getMonsters();
    for (const auto &monster : monsters)
    {
        if (monster->isActive() && monster->getState() == MonsterState::DISEMBODIED)
        {
            return false; // Only one disembodied monster at a time
        }
    }

    return true;
}

void GamePlay::notifyMonsterBecameDisembodied()
{
    // Reset the cooldown timer when a monster becomes disembodied
    disembodiedCooldown = DISEMBODIED_COOLDOWN_TIME;
}

void GamePlay::respawnPlayer()
{
    // Reset player position to starting location
    player.reset(currentLevel.getPlayerStartPosition());

    // Optional: Add brief invincibility period or visual feedback here
    // For now, just respawn at start position
}