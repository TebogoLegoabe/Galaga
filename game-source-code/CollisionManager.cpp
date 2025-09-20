#include "CollisionManager.h"

Monster *CollisionManager::checkPlayerMonsterCollision(const Player &player, const std::vector<std::unique_ptr<Monster>> &monsters)
{
    if (!player.isActive())
        return nullptr;

    Rectangle playerBounds = player.getBounds();

    for (const auto &monster : monsters)
    {
        if (monster && monster->isActive() && !monster->isDead())
        {
            Rectangle monsterBounds = monster->getBounds();
            if (CheckCollisionRecs(playerBounds, monsterBounds))
            {
                return monster.get();
            }
        }
    }

    return nullptr;
}

Monster *CollisionManager::checkHarpoonMonsterCollision(Harpoon &harpoon, const std::vector<std::unique_ptr<Monster>> &monsters, const Grid &grid)
{
    if (!harpoon.isHarpoonActive())
        return nullptr;

    Rectangle harpoonBounds = harpoon.getBounds();

    for (const auto &monster : monsters)
    {
        if (monster && monster->isActive() && !monster->isDead())
        {
            Rectangle monsterBounds = monster->getBounds();
            if (CheckCollisionRecs(harpoonBounds, monsterBounds))
            {
                return monster.get();
            }
        }
    }

    return nullptr;
}

bool CollisionManager::checkHarpoonBounds(const Harpoon &harpoon, const Grid &grid)
{
    if (!harpoon.isHarpoonActive())
        return false;

    // Use the existing hasReachedMaxRange method
    return harpoon.hasReachedMaxRange(grid);
}

// Overloaded methods for raw pointer vectors
Monster *CollisionManager::checkPlayerMonsterCollision(const Player &player, const std::vector<Monster *> &monsters)
{
    if (!player.isActive())
        return nullptr;

    Rectangle playerBounds = player.getBounds();

    for (Monster *monster : monsters)
    {
        if (monster && monster->isActive() && !monster->isDead())
        {
            Rectangle monsterBounds = monster->getBounds();
            if (CheckCollisionRecs(playerBounds, monsterBounds))
            {
                return monster;
            }
        }
    }

    return nullptr;
}

Monster *CollisionManager::checkHarpoonMonsterCollision(Harpoon &harpoon, const std::vector<Monster *> &monsters, const Grid &grid)
{
    if (!harpoon.isHarpoonActive())
        return nullptr;

    Rectangle harpoonBounds = harpoon.getBounds();

    for (Monster *monster : monsters)
    {
        if (monster && monster->isActive() && !monster->isDead())
        {
            Rectangle monsterBounds = monster->getBounds();
            if (CheckCollisionRecs(harpoonBounds, monsterBounds))
            {
                return monster;
            }
        }
    }

    return nullptr;
}