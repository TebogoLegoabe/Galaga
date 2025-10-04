#include "MonsterManager.h"
#include "Fire.h"
#include <cmath>
#include <algorithm>

MonsterManager::MonsterManager() {}

void MonsterManager::initialize(const Level &level, Vector2 playerStartPos)
{
    monsters.clear();

    std::vector<Vector2> spawnPositions = level.getMonsterSpawnPositions();
    addMonstersToEmptyTunnels(spawnPositions, level.getGrid(), playerStartPos);
    ensureMinimumSpawns(spawnPositions, level.getGrid(), playerStartPos);

    createGreenDragons(spawnPositions, 2);
    createRemainingMonsters(spawnPositions);
    removeMonstersTooCloseToPlayer(playerStartPos);
    ensureMinimumGreenDragons(level.getGrid(), playerStartPos, 2);
    ensureMinimumMonsterCount(level.getGrid(), playerStartPos, 3);
}

void MonsterManager::ensureMinimumSpawns(std::vector<Vector2> &spawnPositions,
                                         const Grid &grid, Vector2 playerStartPos)
{
    const int minSpawns = 5;
    while (spawnPositions.size() < minSpawns)
    {
        addMonstersToDistantTunnels(grid, playerStartPos);
        if (monsters.size() > 0)
            break;
    }
}

void MonsterManager::createGreenDragons(const std::vector<Vector2> &spawnPositions, int count)
{
    int created = 0;
    for (size_t i = 0; i < spawnPositions.size() && created < count; ++i)
    {
        monsters.push_back(std::make_unique<GreenDragon>(spawnPositions[i]));
        created++;
    }
}

void MonsterManager::createRemainingMonsters(const std::vector<Vector2> &spawnPositions)
{
    size_t startIndex = std::min(size_t(2), spawnPositions.size());

    for (size_t i = startIndex; i < spawnPositions.size(); ++i)
    {
        int monsterType = rand() % 10;

        if (monsterType < 4) // 40% Red
            monsters.push_back(std::make_unique<RedMonster>(spawnPositions[i]));
        else // 60% Regular
            monsters.push_back(std::make_unique<Monster>(spawnPositions[i], MonsterState::IN_TUNNEL));
    }
}

void MonsterManager::removeMonstersTooCloseToPlayer(Vector2 playerStartPos)
{
    monsters.erase(
        std::remove_if(monsters.begin(), monsters.end(),
                       [&playerStartPos](const std::unique_ptr<Monster> &monster)
                       {
                           Vector2 pos = monster->getPosition();
                           float dist = std::sqrt(std::pow(pos.x - playerStartPos.x, 2) +
                                                  std::pow(pos.y - playerStartPos.y, 2));
                           return dist < 96.0f;
                       }),
        monsters.end());
}

void MonsterManager::ensureMinimumGreenDragons(const Grid &grid, Vector2 playerStartPos, int minCount)
{
    int current = countMonsterType<GreenDragon>();

    while (current < minCount)
    {
        auto distantTunnels = findDistantTunnels(grid, playerStartPos, 128.0f);
        if (distantTunnels.empty())
            break;

        int idx = rand() % distantTunnels.size();
        monsters.push_back(std::make_unique<GreenDragon>(distantTunnels[idx]));
        current++;
    }
}

void MonsterManager::ensureMinimumMonsterCount(const Grid &grid, Vector2 playerStartPos, int minCount)
{
    if (static_cast<int>(monsters.size()) < minCount)
        addMonstersToDistantTunnels(grid, playerStartPos);
}

template <typename T>
int MonsterManager::countMonsterType() const
{
    int count = 0;
    for (const auto &monster : monsters)
    {
        if (dynamic_cast<T *>(monster.get()) != nullptr)
            count++;
    }
    return count;
}

std::vector<Vector2> MonsterManager::findDistantTunnels(const Grid &grid, Vector2 playerStartPos, float minDistance)
{
    std::vector<Vector2> tunnels;

    for (int y = 0; y < grid.getHeight(); y++)
    {
        for (int x = 0; x < grid.getWidth(); x++)
        {
            if (grid.isTunnel(x, y))
            {
                Vector2 worldPos = grid.gridToWorld(x, y);
                float dist = std::sqrt(std::pow(worldPos.x - playerStartPos.x, 2) +
                                       std::pow(worldPos.y - playerStartPos.y, 2));

                if (dist >= minDistance)
                    tunnels.push_back(worldPos);
            }
        }
    }

    return tunnels;
}

void MonsterManager::update(const Player &player, Grid &grid, bool canBecomeDisembodied,
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