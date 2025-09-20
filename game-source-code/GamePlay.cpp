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

        // Handle harpoon shooting
        if (InputHandler::isActionPressed() && player.canShoot())
        {
            player.shootHarpoon();
        }
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
    for (auto &monster : monsters)
    {
        if (monster && monster->isActive())
        {
            monster->draw();
        }
    }

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
    return monsters;
}

bool GamePlay::allMonstersDead() const
{
    for (const auto &monster : monsters)
    {
        if (monster && monster->isActive() && !monster->isDead())
        {
            return false;
        }
    }
    return true;
}

void GamePlay::updateGameLogic()
{
    // Check collisions
    checkCollisions();

    // Check win condition - all monsters dead
    if (allMonstersDead() && !gameOver)
    {
        gameOver = true;
        levelComplete = true;
        playerWon = true;
    }
}

void GamePlay::updateMonsters()
{
    for (auto &monster : monsters)
    {
        if (monster && monster->isActive() && !monster->isDead())
        {
            monster->updateAI(currentLevel.getGrid(), player.getPosition());
            monster->update();
        }
    }
}

void GamePlay::checkCollisions()
{
    // Check player-monster collisions
    Monster *collidingMonster = CollisionManager::checkPlayerMonsterCollision(player, monsters);
    if (collidingMonster)
    {
        // Player loses
        gameOver = true;
        playerWon = false;
        return;
    }

    // Check harpoon-monster collisions
    Monster *hitMonster = CollisionManager::checkHarpoonMonsterCollision(player.getHarpoon(), monsters, currentLevel.getGrid());
    if (hitMonster)
    {
        // Monster is hit by harpoon
        hitMonster->kill();
        player.getHarpoon().stop();
    }

    // Check if harpoon is out of bounds
    if (CollisionManager::checkHarpoonBounds(player.getHarpoon(), currentLevel.getGrid()))
    {
        player.getHarpoon().stop();
    }
}

void GamePlay::initializeMonsters()
{
    // Clear existing monsters
    monsters.clear();

    // Get monster spawn positions from level
    std::vector<Vector2> spawnPositions = currentLevel.getMonsterSpawnPositions();

    // Create monsters at spawn positions
    for (size_t i = 0; i < spawnPositions.size(); ++i)
    {
        // Mix of red monsters and green dragons
        bool isGreen = (i % 3 == 0); // Every third monster is a green dragon
        auto monster = std::make_unique<Monster>(spawnPositions[i], isGreen);
        monsters.push_back(std::move(monster));
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
    for (const auto &monster : monsters)
    {
        if (monster && monster->isActive() && !monster->isDead())
        {
            aliveMonsters++;
        }
    }
    const char *monsterText = TextFormat("Monsters Remaining: %d", aliveMonsters);
    DrawText(monsterText, 10, 55, 15, WHITE);

    // Draw harpoon status
    const char *harpoonStatus = player.canShoot() ? "Harpoon Ready" : "Harpoon Fired";
    Color harpoonColor = player.canShoot() ? GREEN : RED;
    DrawText(harpoonStatus, 10, 75, 15, harpoonColor);

    // Draw controls
    const char *controls = "Use ARROW KEYS or WASD to move and dig tunnels";
    DrawText(controls, 10, GetScreenHeight() - 85, 15, WHITE);

    const char *shootInfo = "Press SPACE to shoot harpoon at monsters";
    DrawText(shootInfo, 10, GetScreenHeight() - 65, 15, WHITE);

    const char *instruction = "Press ESC to return to menu";
    DrawText(instruction, 10, GetScreenHeight() - 45, 15, WHITE);

    const char *objective = "Destroy all monsters to win!";
    DrawText(objective, 10, GetScreenHeight() - 25, 15, YELLOW);
}

void GamePlay::handlePlayerMovement()
{
    Direction moveDirection = InputHandler::getDirectionInput();

    if (moveDirection != Direction::NONE)
    {
        player.move(moveDirection, currentLevel.getGrid());
    }
}