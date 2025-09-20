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

std::vector<std::unique_ptr<Monster>> &GamePlay::getMonsters()
{
    return monsters;
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
            checkHarpoonMonsterCollisions();
        }
    }

    // Check for collisions between player and monsters
    checkPlayerMonsterCollisions();

    // Check if all monsters are dead (win condition)
    if (areAllMonstersDead())
    {
        gameOver = true;
        levelComplete = true;
        playerWon = true;
    }
}

void GamePlay::checkHarpoonMonsterCollisions()
{
    Harpoon &harpoon = player.getHarpoon();
    if (!harpoon.isHarpoonActive())
        return;

    Rectangle harpoonBounds = harpoon.getBounds();

    for (auto &monster : monsters)
    {
        if (monster->isActive() && !monster->isDead())
        {
            Rectangle monsterBounds = monster->getBounds();
            if (CheckCollisionRecs(harpoonBounds, monsterBounds))
            {
                // Monster hit by harpoon - kill it
                monster->setState(MonsterState::DEAD);

                // Deactivate the harpoon
                harpoon.deactivate();

                return; // Only hit one monster per frame
            }
        }
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
        if (monster->isActive() && !monster->isDead())
            aliveMonsters++;
    }
    const char *monsterText = TextFormat("Monsters Remaining: %d", aliveMonsters);
    DrawText(monsterText, 10, 55, 15, WHITE);

    // Draw controls
    const char *controls = "ARROW KEYS/WASD: Move and dig | SPACE: Shoot harpoon";
    DrawText(controls, 10, GetScreenHeight() - 65, 15, WHITE);

    const char *harpoonStatus = player.canShoot() ? "Harpoon: READY" : "Harpoon: RELOADING...";
    Color harpoonColor = player.canShoot() ? GREEN : ORANGE;
    DrawText(harpoonStatus, 10, 75, 15, harpoonColor);

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

    // Handle shooting
    if (InputHandler::isActionPressed())
    {
        player.shoot();
    }
}

void GamePlay::initializeMonsters()
{
    monsters.clear();

    // Get monster spawn positions from the level
    std::vector<Vector2> spawnPositions = currentLevel.getMonsterSpawnPositions();

    // Create monsters at spawn positions
    for (const Vector2 &spawnPos : spawnPositions)
    {
        auto monster = std::make_unique<Monster>(spawnPos, MonsterState::IN_TUNNEL);
        monsters.push_back(std::move(monster));
    }

    // Ensure at least one monster exists in the same tunnel as the player, but not adjacent
    if (!monsters.empty())
    {
        Vector2 playerStart = currentLevel.getPlayerStartPosition();
        Vector2 playerGridPos = currentLevel.getGrid().worldToGrid(playerStart);
        int tileSize = currentLevel.getGrid().getTileSize();

        // Try to find a position in the same tunnel but at least 3 tiles away from player
        std::vector<Vector2> validPositions;

        // Check horizontal tunnel positions (same row, different columns)
        for (int offsetX = -10; offsetX <= 10; offsetX++)
        {
            if (offsetX == 0)
                continue; // Skip player's exact position

            int testGridX = static_cast<int>(playerGridPos.x) + offsetX;
            int testGridY = static_cast<int>(playerGridPos.y);

            // Must be at least 3 tiles away from player
            if (std::abs(offsetX) >= 3 &&
                currentLevel.getGrid().isValidPosition(testGridX, testGridY) &&
                currentLevel.getGrid().isTunnel(testGridX, testGridY))
            {
                Vector2 worldPos = currentLevel.getGrid().gridToWorld(testGridX, testGridY);
                validPositions.push_back(worldPos);
            }
        }

        // Check vertical tunnel positions (same column, different rows)
        for (int offsetY = -10; offsetY <= 10; offsetY++)
        {
            if (offsetY == 0)
                continue; // Skip player's exact position

            int testGridX = static_cast<int>(playerGridPos.x);
            int testGridY = static_cast<int>(playerGridPos.y) + offsetY;

            // Must be at least 3 tiles away from player
            if (std::abs(offsetY) >= 3 &&
                currentLevel.getGrid().isValidPosition(testGridX, testGridY) &&
                currentLevel.getGrid().isTunnel(testGridX, testGridY))
            {
                Vector2 worldPos = currentLevel.getGrid().gridToWorld(testGridX, testGridY);
                validPositions.push_back(worldPos);
            }
        }

        // If we found valid positions, place the first monster there
        if (!validPositions.empty())
        {
            // Choose a random valid position
            int randomIndex = rand() % validPositions.size();
            monsters[0]->setPosition(validPositions[randomIndex]);
        }
        // Otherwise, keep the monster at its original spawn position (which should be far from player)
    }

    // Ensure no monsters are too close to player (safety check)
    Vector2 playerStart = currentLevel.getPlayerStartPosition();
    for (auto &monster : monsters)
    {
        Vector2 monsterPos = monster->getPosition();
        float distance = std::sqrt(std::pow(monsterPos.x - playerStart.x, 2) +
                                   std::pow(monsterPos.y - playerStart.y, 2));

        // If monster is too close (less than 2 tiles), move it to a default safe position
        if (distance < 64.0f) // 2 tiles * 32 pixels
        {
            // Move to a predefined safe spawn position
            std::vector<Vector2> safeSpawns = currentLevel.getMonsterSpawnPositions();
            if (!safeSpawns.empty())
            {
                // Find the spawn position furthest from player
                Vector2 furthestSpawn = safeSpawns[0];
                float maxDistance = 0.0f;

                for (const Vector2 &spawn : safeSpawns)
                {
                    float spawnDistance = std::sqrt(std::pow(spawn.x - playerStart.x, 2) +
                                                    std::pow(spawn.y - playerStart.y, 2));
                    if (spawnDistance > maxDistance)
                    {
                        maxDistance = spawnDistance;
                        furthestSpawn = spawn;
                    }
                }

                monster->setPosition(furthestSpawn);
            }
        }
    }
}

void GamePlay::updateMonsters()
{
    for (auto &monster : monsters)
    {
        if (monster->isActive() && !monster->isDead())
        {
            // Update monster
            monster->update();

            // Update monster AI to chase player
            monster->updateAI(player, currentLevel.getGrid());
        }
    }
}

void GamePlay::drawMonsters()
{
    for (const auto &monster : monsters)
    {
        if (monster->isActive())
        {
            monster->draw();
        }
    }
}

void GamePlay::checkPlayerMonsterCollisions()
{
    for (auto &monster : monsters)
    {
        if (monster->isActive() && !monster->isDead())
        {
            if (checkCollision(player, *monster))
            {
                // Player collided with monster - game over
                gameOver = true;
                playerWon = false;
                return;
            }
        }
    }
}

bool GamePlay::checkCollision(const GameObject &obj1, const GameObject &obj2) const
{
    Rectangle bounds1 = obj1.getBounds();
    Rectangle bounds2 = obj2.getBounds();

    return CheckCollisionRecs(bounds1, bounds2);
}

bool GamePlay::areAllMonstersDead() const
{
    for (const auto &monster : monsters)
    {
        if (monster->isActive() && !monster->isDead())
        {
            return false;
        }
    }
    return true;
}