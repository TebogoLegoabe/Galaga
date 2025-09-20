#include "GamePlay.h"

GamePlay::GamePlay()
    : gameOver(false), levelComplete(false), playerWon(false)
{
}

GamePlay::~GamePlay()
{
    // Cleanup will be handled automatically by smart pointers
}

void GamePlay::init()
{
    // Initialize the level
    currentLevel.initializeDefault();

    // Initialize the player at the level's start position
    player.reset(currentLevel.getPlayerStartPosition());

    // Initialize monsters
    initializeMonsters();

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

        // Update monsters
        updateMonsters();

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
    drawMonsters();

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

const std::vector<std::unique_ptr<Monster>> &GamePlay::getMonsters() const
{
    return monsters;
}

void GamePlay::updateGameLogic()
{
    // Check player-monster collisions
    std::vector<Monster *> monsterPtrs;
    for (const auto &monster : monsters)
    {
        monsterPtrs.push_back(monster.get());
    }

    if (CollisionManager::checkPlayerMonsterCollision(player, monsterPtrs))
    {
        // Player hit by monster - game over
        gameOver = true;
        playerWon = false;
        return;
    }

    // Check win condition
    if (checkWinCondition())
    {
        levelComplete = true;
        playerWon = true;
        gameOver = true;
    }
}

void GamePlay::drawHUD()
{
    // Draw basic HUD elements
    const char *title = "DIG DUG";
    DrawText(title, 10, 10, 20, ORANGE);

    // Draw monster count
    int aliveMonsters = 0;
    for (const auto &monster : monsters)
    {
        if (monster->isActive() && monster->getState() != MonsterState::DEAD)
            aliveMonsters++;
    }
    const char *monsterText = TextFormat("Monsters: %d", aliveMonsters);
    DrawText(monsterText, 10, 35, 15, WHITE);
}

void GamePlay::handlePlayerMovement()
{
    Direction moveDirection = InputHandler::getDirectionInput();

    if (moveDirection != Direction::NONE)
    {
        player.move(moveDirection, currentLevel.getGrid());
    }
}

void GamePlay::initializeMonsters()
{
    // Clear existing monsters
    monsters.clear();

    // Get monster spawn positions from level
    std::vector<Vector2> spawnPositions = currentLevel.getMonsterSpawnPositions();

    // Create red monsters at spawn positions
    for (const Vector2 &spawnPos : spawnPositions)
    {
        auto monster = std::make_unique<RedMonster>(spawnPos);
        monsters.push_back(std::move(monster));
    }
}

void GamePlay::updateMonsters()
{
    Vector2 playerPos = player.getPosition();

    for (auto &monster : monsters)
    {
        if (monster->isActive() && monster->getState() != MonsterState::DEAD)
        {
            // Update monster AI with player position
            RedMonster *redMonster = dynamic_cast<RedMonster *>(monster.get());
            if (redMonster)
            {
                redMonster->updateAI(playerPos, currentLevel.getGrid());
            }

            // Update monster
            monster->update();
        }
    }
}

void GamePlay::drawMonsters()
{
    for (const auto &monster : monsters)
    {
        if (monster->isActive() && monster->getState() != MonsterState::DEAD)
        {
            monster->draw();
        }
    }
}

bool GamePlay::checkWinCondition() const
{
    for (const auto &monster : monsters)
    {
        if (monster->isActive() && monster->getState() != MonsterState::DEAD)
        {
            return false; // Still have alive monsters
        }
    }
    return true; // All monsters defeated
}