#include "MonsterManager.h"
#include "Fire.h"
#include <cmath>
#include <algorithm>

MonsterManager::MonsterManager()
{
}

void MonsterManager::initialize(const Level &level, Vector2 playerStartPos)
{
    monsters.clear();

    // Get monster spawn positions from the level
    std::vector<Vector2> spawnPositions = level.getMonsterSpawnPositions();

    // Add additional monster spawns in empty tunnels
    addMonstersToEmptyTunnels(spawnPositions, level.getGrid(), playerStartPos);

    // Ensure we have enough spawn positions for at least 2 green dragons
    int minSpawns = 5; // We want at least 5 monsters total to have variety
    while (spawnPositions.size() < minSpawns)
    {
        addMonstersToDistantTunnels(level.getGrid(), playerStartPos);
        // Add the new monsters' positions to spawnPositions for the next check
        if (monsters.size() > 0)
        {
            break; // addMonstersToDistantTunnels creates monsters directly
        }
    }

    // First, create exactly 2 green dragons from the first spawn positions
    int greenDragonsCreated = 0;
    size_t i = 0;
    for (; i < spawnPositions.size() && greenDragonsCreated < 2; ++i)
    {
        const Vector2 &spawnPos = spawnPositions[i];
        auto greenDragon = std::make_unique<GreenDragon>(spawnPos);
        monsters.push_back(std::move(greenDragon));
        greenDragonsCreated++;
    }

    // Create remaining monsters at other spawn positions with variety
    for (; i < spawnPositions.size(); ++i)
    {
        const Vector2 &spawnPos = spawnPositions[i];

        // Create a mix of different monster types (no more green dragons needed)
        int monsterType = rand() % 10; // Random number 0-9

        if (monsterType < 4) // 40% Red Monsters (aggressive)
        {
            auto redMonster = std::make_unique<RedMonster>(spawnPos);
            monsters.push_back(std::move(redMonster));
        }
        else // 60% Regular Monsters (base enemy)
        {
            auto monster = std::make_unique<Monster>(spawnPos, MonsterState::IN_TUNNEL);
            monsters.push_back(std::move(monster));
        }
    }

    // Remove monsters that are too close to player (safety check)
    monsters.erase(
        std::remove_if(monsters.begin(), monsters.end(),
                       [&playerStartPos](const std::unique_ptr<Monster> &monster)
                       {
                           Vector2 monsterPos = monster->getPosition();
                           float distance = std::sqrt(std::pow(monsterPos.x - playerStartPos.x, 2) +
                                                      std::pow(monsterPos.y - playerStartPos.y, 2));
                           return distance < 96.0f; // Remove if closer than 3 tiles
                       }),
        monsters.end());

    // If we accidentally removed green dragons, ensure we still have at least 2
    int currentGreenDragons = 0;
    for (const auto &monster : monsters)
    {
        if (dynamic_cast<GreenDragon *>(monster.get()) != nullptr)
        {
            currentGreenDragons++;
        }
    }

    // Add more green dragons if needed
    while (currentGreenDragons < 2)
    {
        // Find a distant tunnel position
        std::vector<Vector2> distantTunnels;
        const Grid &grid = level.getGrid();

        for (int y = 0; y < grid.getHeight(); y++)
        {
            for (int x = 0; x < grid.getWidth(); x++)
            {
                if (grid.isTunnel(x, y))
                {
                    Vector2 worldPos = grid.gridToWorld(x, y);
                    float distance = std::sqrt(std::pow(worldPos.x - playerStartPos.x, 2) +
                                               std::pow(worldPos.y - playerStartPos.y, 2));

                    if (distance >= 128.0f) // 4+ tiles away
                    {
                        distantTunnels.push_back(worldPos);
                    }
                }
            }
        }

        if (!distantTunnels.empty())
        {
            int randomIndex = rand() % distantTunnels.size();
            Vector2 spawnPos = distantTunnels[randomIndex];
            auto greenDragon = std::make_unique<GreenDragon>(spawnPos);
            monsters.push_back(std::move(greenDragon));
            currentGreenDragons++;
        }
        else
        {
            break; // Can't find suitable positions
        }
    }

    // Ensure minimum of 3 monsters total
    if (monsters.size() < 3)
    {
        addMonstersToDistantTunnels(level.getGrid(), playerStartPos);
    }
}

void MonsterManager::update(const Player &player, const Grid &grid, bool canBecomeDisembodied,
                            std::function<void()> notifyDisembodied)
{
    for (auto &monster : monsters)
    {
        if (monster->isActive() && !monster->isDead())
        {
            // Update monster (this calls Monster::update() which updates movement)
            monster->update();

            // Check if this is a GreenDragon and update its AI specifically
            GreenDragon *dragon = dynamic_cast<GreenDragon *>(monster.get());
            if (dragon)
            {
                // Green dragons use their own AI
                dragon->updateAI(player, grid, canBecomeDisembodied, notifyDisembodied);
            }
            else
            {
                // Regular monsters and red monsters use base Monster AI
                monster->updateAI(player, grid, canBecomeDisembodied, notifyDisembodied);
            }
        }
    }
}

void MonsterManager::draw()
{
    for (const auto &monster : monsters)
    {
        if (monster->isActive())
        {
            monster->draw();

            // Explicitly draw fire for green dragons
            // (The GreenDragon::draw() should handle this, but let's be certain)
            GreenDragon *dragon = dynamic_cast<GreenDragon *>(monster.get());
            if (dragon)
            {
                Fire &fire = dragon->getFire();
                if (fire.isFireActive())
                {
                    fire.draw();
                }
            }
        }
    }
}

std::vector<std::unique_ptr<Monster>> &MonsterManager::getMonsters()
{
    return monsters;
}

const std::vector<std::unique_ptr<Monster>> &MonsterManager::getMonsters() const
{
    return monsters;
}

bool MonsterManager::areAllMonstersDead() const
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

void MonsterManager::clear()
{
    monsters.clear();
}

void MonsterManager::addMonstersToEmptyTunnels(std::vector<Vector2> &spawnPositions,
                                               const Grid &grid, Vector2 playerStart)
{
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
                        if (rand() % 4 == 0) // 25% chance to place monster
                        {
                            spawnPositions.push_back(worldPos);
                        }
                    }
                }
            }
        }
    }
}

void MonsterManager::addMonstersToDistantTunnels(const Grid &grid, Vector2 playerStart)
{
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
    int monstersToAdd = 3 - static_cast<int>(monsters.size());
    for (int i = 0; i < monstersToAdd && i < static_cast<int>(distantTunnels.size()); i++)
    {
        int randomIndex = rand() % distantTunnels.size();
        Vector2 spawnPos = distantTunnels[randomIndex];

        // Create a mix of monster types
        int monsterType = rand() % 10; // Random number 0-9

        if (monsterType < 2) // 20% Green Dragons
        {
            auto greenDragon = std::make_unique<GreenDragon>(spawnPos);
            monsters.push_back(std::move(greenDragon));
        }
        else if (monsterType < 5) // 30% Red Monsters
        {
            auto redMonster = std::make_unique<RedMonster>(spawnPos);
            monsters.push_back(std::move(redMonster));
        }
        else // 50% Regular Monsters
        {
            auto monster = std::make_unique<Monster>(spawnPos, MonsterState::IN_TUNNEL);
            monsters.push_back(std::move(monster));
        }

        // Remove this position so we don't spawn multiple monsters at the same spot
        distantTunnels.erase(distantTunnels.begin() + randomIndex);
    }
}