#include "CollisionManager.h"

Monster *CollisionManager::checkPlayerMonsterCollision(const Player &player, const std::vector<std::unique_ptr<Monster>> &monsters)
{
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
    // Use getBounds() method instead of accessing private isActive
    Rectangle harpoonBounds = harpoon.getBounds();

    // If harpoon bounds are empty/invalid, it's probably not active
    if (harpoonBounds.width <= 0 || harpoonBounds.height <= 0)
        return nullptr;

    for (const auto &monster : monsters)
    {
        if (monster && monster->isActive() && !monster->isDead())
        {
            Rectangle monsterBounds = monster->getBounds();
            if (CheckCollisionRecs(harpoonBounds, monsterBounds))
            {
                // Kill the monster
                monster->setState(MonsterState::DEAD);

                // Try to deactivate the harpoon (if it has a public method)
                // harpoon.setActive(false); // This might not exist, so we'll skip it

                return monster.get();
            }
        }
    }
    return nullptr;
}

Monster *CollisionManager::checkPlayerMonsterCollision(const Player &player, const std::vector<Monster *> &monsters)
{
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
    // Use getBounds() method instead of accessing private isActive
    Rectangle harpoonBounds = harpoon.getBounds();

    // If harpoon bounds are empty/invalid, it's probably not active
    if (harpoonBounds.width <= 0 || harpoonBounds.height <= 0)
        return nullptr;

    for (Monster *monster : monsters)
    {
        if (monster && monster->isActive() && !monster->isDead())
        {
            Rectangle monsterBounds = monster->getBounds();
            if (CheckCollisionRecs(harpoonBounds, monsterBounds))
            {
                // Kill the monster
                monster->setState(MonsterState::DEAD);

                // Try to deactivate the harpoon (if it has a public method)
                // harpoon.setActive(false); // This might not exist, so we'll skip it

                return monster;
            }
        }
    }
    return nullptr;
}