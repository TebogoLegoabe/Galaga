#include "GamePlay.h"
#include "GreenDragon.h"
#include "Fire.h"

const float GamePlay::DISEMBODIED_COOLDOWN_TIME = 3.0f; // 3 seconds between disembodied transitions

GamePlay::GamePlay()
    : gameOver(false), levelComplete(false), playerWon(false), disembodiedCooldown(0.0f)
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

    // Reset lives only when starting a completely new game
    player.resetLives();

    // Initialize monsters (red monsters and green dragons)
    initializeMonsters();
    initializeGreenDragons();

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

        // Update monsters and dragons
        updateMonsters();
        updateGreenDragons();

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

    // Draw monsters and dragons
    drawMonsters();
    drawGreenDragons();

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

std::vector<std::unique_ptr<GreenDragon>> &GamePlay::getGreenDragons()
{
    return greenDragons;
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

    // Check for fire-player collisions
    checkFirePlayerCollisions();

    // Check for collisions between player and monsters
    checkPlayerMonsterCollisions();

    // Check if all monsters and dragons are dead (win condition)
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

    // Check collisions with red monsters
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

    // Check collisions with green dragons
    for (auto &dragon : greenDragons)
    {
        if (dragon->isActive() && !dragon->isDead())
        {
            Rectangle dragonBounds = dragon->getBounds();
            if (CheckCollisionRecs(harpoonBounds, dragonBounds))
            {
                // Dragon hit by harpoon - kill it
                dragon->setState(MonsterState::DEAD);

                // Deactivate the harpoon
                harpoon.deactivate();

                return; // Only hit one monster per frame
            }
        }
    }
}

void GamePlay::checkFirePlayerCollisions()
{
    Rectangle playerBounds = player.getBounds();

    // Check collisions with fire from all dragons
    for (auto &dragon : greenDragons)
    {
        if (dragon->isActive() && !dragon->isDead())
        {
            Fire &fire = dragon->getFire();
            if (fire.isFireActive())
            {
                Rectangle fireBounds = fire.getBounds();
                if (CheckCollisionRecs(playerBounds, fireBounds))
                {
                    // Player hit by fire - lose a life
                    bool stillAlive = player.loseLife();

                    // Deactivate the fire
                    fire.deactivate();

                    if (!stillAlive)
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
                    return;
                }
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

    // Draw monster count (including dragons)
    int aliveMonsters = 0;
    for (const auto &monster : monsters)
    {
        if (monster->isActive() && !monster->isDead())
            aliveMonsters++;
    }
    for (const auto &dragon : greenDragons)
    {
        if (dragon->isActive() && !dragon->isDead())
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

    // Draw controls
    const char *controls = "ARROW KEYS/WASD: Move and dig | SPACE: Shoot harpoon";
    DrawText(controls, 10, GetScreenHeight() - 65, 15, WHITE);

    const char *harpoonStatus = player.canShoot() ? "Harpoon: READY" : "Harpoon: RELOADING...";
    Color harpoonColor = player.canShoot() ? GREEN : ORANGE;
    DrawText(harpoonStatus, 10, 95, 15, harpoonColor);

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

    // Reserve some positions for green dragons (split spawns)
    size_t dragonsCount = std::min(spawnPositions.size() / 2, static_cast<size_t>(2)); // Max 2 dragons

    // Use remaining positions for red monsters
    for (size_t i = dragonsCount; i < spawnPositions.size(); i++)
    {
        auto monster = std::make_unique<Monster>(spawnPositions[i], MonsterState::IN_TUNNEL);
        monsters.push_back(std::move(monster));
    }

    // Add additional monster spawns in empty tunnels if needed
    addMonstersToEmptyTunnels(spawnPositions);

    // Ensure we have at least 2 red monsters
    if (monsters.size() < 2)
    {
        addMonstersToDistantTunnels();
    }
}

void GamePlay::initializeGreenDragons()
{
    greenDragons.clear();

    // Get spawn positions from the level
    std::vector<Vector2> spawnPositions = currentLevel.getMonsterSpawnPositions();

    // Use first few positions for green dragons
    size_t dragonsCount = std::min(spawnPositions.size() / 2, static_cast<size_t>(2)); // Max 2 dragons

    for (size_t i = 0; i < dragonsCount && i < spawnPositions.size(); i++)
    {
        auto dragon = std::make_unique<GreenDragon>(spawnPositions[i]);
        greenDragons.push_back(std::move(dragon));
    }

    // Add additional green dragon spawns if needed
    addGreenDragonsToTunnels(spawnPositions);

    // Ensure we have at least 1 green dragon
    if (greenDragons.empty())
    {
        // Find a suitable spawn position far from player
        Vector2 playerStart = currentLevel.getPlayerStartPosition();
        const Grid &grid = currentLevel.getGrid();

        // Try to find a tunnel position that's far from player
        for (int y = 0; y < grid.getHeight(); y++)
        {
            for (int x = 0; x < grid.getWidth(); x++)
            {
                if (grid.isTunnel(x, y))
                {
                    Vector2 worldPos = grid.gridToWorld(x, y);
                    float distance = std::sqrt(std::pow(worldPos.x - playerStart.x, 2) +
                                               std::pow(worldPos.y - playerStart.y, 2));

                    if (distance >= 128.0f) // At least 4 tiles away
                    {
                        auto dragon = std::make_unique<GreenDragon>(worldPos);
                        greenDragons.push_back(std::move(dragon));
                        break;
                    }
                }
            }
            if (!greenDragons.empty())
                break;
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

            // Update monster AI with disembodied restrictions
            bool canBecomeDisembodied = canMonsterBecomeDisembodied();
            monster->updateAI(player, currentLevel.getGrid(), canBecomeDisembodied,
                              [this]()
                              { notifyMonsterBecameDisembodied(); });
        }
    }
}

void GamePlay::updateGreenDragons()
{
    for (auto &dragon : greenDragons)
    {
        if (dragon->isActive() && !dragon->isDead())
        {
            // Update dragon
            dragon->update();

            // Check if dragon fire should be destroyed
            Fire &fire = dragon->getFire();
            if (fire.isFireActive() && fire.shouldDestroy(currentLevel.getGrid()))
            {
                fire.deactivate();
            }

            // Update dragon AI with disembodied restrictions
            bool canBecomeDisembodied = canMonsterBecomeDisembodied();
            dragon->updateAI(player, currentLevel.getGrid(), canBecomeDisembodied,
                             [this]()
                             { notifyMonsterBecameDisembodied(); });
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

void GamePlay::drawGreenDragons()
{
    for (const auto &dragon : greenDragons)
    {
        if (dragon->isActive())
        {
            dragon->draw();
        }
    }
}

void GamePlay::checkPlayerMonsterCollisions()
{
    // Check collisions with red monsters
    for (auto &monster : monsters)
    {
        if (monster->isActive() && !monster->isDead())
        {
            if (checkCollision(player, *monster))
            {
                // Player collided with monster - lose a life
                bool stillAlive = player.loseLife();

                if (!stillAlive)
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
                return;
            }
        }
    }

    // Check collisions with green dragons
    for (auto &dragon : greenDragons)
    {
        if (dragon->isActive() && !dragon->isDead())
        {
            if (checkCollision(player, *dragon))
            {
                // Player collided with dragon - lose a life
                bool stillAlive = player.loseLife();

                if (!stillAlive)
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
    // Check red monsters
    for (const auto &monster : monsters)
    {
        if (monster->isActive() && !monster->isDead())
        {
            return false;
        }
    }

    // Check green dragons
    for (const auto &dragon : greenDragons)
    {
        if (dragon->isActive() && !dragon->isDead())
        {
            return false;
        }
    }

    return true;
}

void GamePlay::addMonstersToEmptyTunnels(std::vector<Vector2> &spawnPositions)
{
    const Grid &grid = currentLevel.getGrid();
    Vector2 playerStart = currentLevel.getPlayerStartPosition();
    Vector2 playerGridPos = grid.worldToGrid(playerStart);

    // Scan the entire grid for tunnel positions
    for (int y = 0; y < grid.getHeight(); y++)
    {
        for (int x = 0; x < grid.getWidth(); x++)
        {
            if (grid.isTunnel(x, y))
            {
                Vector2 worldPos = grid.gridToWorld(x, y);

                // Calculate distance from player
                float distance = std::sqrt(std::pow(worldPos.x - playerStart.x, 2) +
                                           std::pow(worldPos.y - playerStart.y, 2));

                // Only add monsters that are far enough from player (3+ tiles away)
                if (distance >= 96.0f)
                {
                    // Check if this position is already occupied
                    bool occupied = false;
                    for (const Vector2 &existing : spawnPositions)
                    {
                        float distToExisting = std::sqrt(std::pow(worldPos.x - existing.x, 2) +
                                                         std::pow(worldPos.y - existing.y, 2));
                        if (distToExisting < 32.0f) // Less than 1 tile away
                        {
                            occupied = true;
                            break;
                        }
                    }

                    if (!occupied)
                    {
                        // Add some randomness - don't fill every tunnel
                        if (rand() % 5 == 0) // 20% chance to place monster
                        {
                            auto monster = std::make_unique<Monster>(worldPos, MonsterState::IN_TUNNEL);
                            monsters.push_back(std::move(monster));
                        }
                    }
                }
            }
        }
    }
}

void GamePlay::addMonstersToDistantTunnels()
{
    const Grid &grid = currentLevel.getGrid();
    Vector2 playerStart = currentLevel.getPlayerStartPosition();

    std::vector<Vector2> distantTunnels;

    // Find tunnel positions that are far from player
    for (int y = 0; y < grid.getHeight(); y++)
    {
        for (int x = 0; x < grid.getWidth(); x++)
        {
            if (grid.isTunnel(x, y))
            {
                Vector2 worldPos = grid.gridToWorld(x, y);

                // Calculate distance from player
                float distance = std::sqrt(std::pow(worldPos.x - playerStart.x, 2) +
                                           std::pow(worldPos.y - playerStart.y, 2));

                // Only consider positions that are far from player
                if (distance >= 128.0f) // 4+ tiles away
                {
                    // Check if position is not occupied by existing monsters
                    bool occupied = false;
                    for (const auto &monster : monsters)
                    {
                        Vector2 monsterPos = monster->getPosition();
                        float distToMonster = std::sqrt(std::pow(worldPos.x - monsterPos.x, 2) +
                                                        std::pow(worldPos.y - monsterPos.y, 2));
                        if (distToMonster < 64.0f) // Less than 2 tiles away
                        {
                            occupied = true;
                            break;
                        }
                    }

                    if (!occupied)
                    {
                        distantTunnels.push_back(worldPos);
                    }
                }
            }
        }
    }

    // Add monsters to some distant tunnels
    int monstersToAdd = 2 - static_cast<int>(monsters.size());
    for (int i = 0; i < monstersToAdd && i < static_cast<int>(distantTunnels.size()); i++)
    {
        int randomIndex = rand() % distantTunnels.size();
        Vector2 spawnPos = distantTunnels[randomIndex];

        auto monster = std::make_unique<Monster>(spawnPos, MonsterState::IN_TUNNEL);
        monsters.push_back(std::move(monster));

        // Remove this position so we don't spawn multiple monsters at the same spot
        distantTunnels.erase(distantTunnels.begin() + randomIndex);
    }
}

void GamePlay::addGreenDragonsToTunnels(std::vector<Vector2> &spawnPositions)
{
    const Grid &grid = currentLevel.getGrid();
    Vector2 playerStart = currentLevel.getPlayerStartPosition();

    // Find suitable tunnel positions for additional dragons
    for (int y = 0; y < grid.getHeight(); y++)
    {
        for (int x = 0; x < grid.getWidth(); x++)
        {
            if (grid.isTunnel(x, y))
            {
                Vector2 worldPos = grid.gridToWorld(x, y);

                // Calculate distance from player
                float distance = std::sqrt(std::pow(worldPos.x - playerStart.x, 2) +
                                           std::pow(worldPos.y - playerStart.y, 2));

                // Only add dragons that are far enough from player
                if (distance >= 160.0f) // 5+ tiles away (dragons are more dangerous)
                {
                    // Check if this position is already occupied
                    bool occupied = false;
                    for (const Vector2 &existing : spawnPositions)
                    {
                        float distToExisting = std::sqrt(std::pow(worldPos.x - existing.x, 2) +
                                                         std::pow(worldPos.y - existing.y, 2));
                        if (distToExisting < 64.0f) // Less than 2 tiles away
                        {
                            occupied = true;
                            break;
                        }
                    }

                    if (!occupied && greenDragons.size() < 2) // Max 2 dragons
                    {
                        // Lower chance to place dragons (they're more dangerous)
                        if (rand() % 8 == 0) // 12.5% chance
                        {
                            auto dragon = std::make_unique<GreenDragon>(worldPos);
                            greenDragons.push_back(std::move(dragon));
                        }
                    }
                }
            }
        }
    }
}

bool GamePlay::canMonsterBecomeDisembodied() const
{
    // Check if cooldown allows a new disembodied monster
    if (disembodiedCooldown > 0.0f)
        return false;

    // Check if there's already a disembodied monster (red monsters or dragons)
    for (const auto &monster : monsters)
    {
        if (monster->isActive() && monster->getState() == MonsterState::DISEMBODIED)
        {
            return false; // Only one disembodied monster at a time
        }
    }

    for (const auto &dragon : greenDragons)
    {
        if (dragon->isActive() && dragon->getState() == MonsterState::DISEMBODIED)
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
}