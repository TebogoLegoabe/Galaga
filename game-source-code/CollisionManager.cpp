#include "CollisionManager.h"

bool CollisionManager::checkPlayerMonsterCollision(const Player &player,
                                                   const std::vector<Monster *> &monsters)
{
    if (!player.isActive())
        return false;

    Rectangle playerBounds = player.getBounds();

    for (const Monster *monster : monsters)
    {
        if (monster && monster->isActive() && monster->getState() != MonsterState::DEAD)
        {
            Rectangle monsterBounds = monster->getBounds();
            if (checkRectangleCollision(playerBounds, monsterBounds))
            {
                return true;
            }
        }
    }

    return false;
}

Monster *CollisionManager::checkHarpoonMonsterCollision(Harpoon &harpoon,
                                                        const std::vector<Monster *> &monsters,
                                                        const Grid &grid)
{
    if (!harpoon.isActive() || harpoon.getState() != HarpoonState::FLYING)
        return nullptr;

    Rectangle harpoonBounds = harpoon.getBounds();

    for (Monster *monster : monsters)
    {
        if (monster && monster->isActive() && monster->getState() != MonsterState::DEAD)
        {
            Rectangle monsterBounds = monster->getBounds();
            if (checkRectangleCollision(harpoonBounds, monsterBounds))
            {
                return monster;
            }
        }
    }

    return nullptr;
}

bool CollisionManager::checkHarpoonBounds(const Harpoon &harpoon, const Grid &grid)
{
    if (!harpoon.isActive())
        return false;

    return !harpoon.isWithinBounds(grid);
}

bool CollisionManager::checkCollision(const GameObject &obj1, const GameObject &obj2)
{
    if (!obj1.isActive() || !obj2.isActive())
        return false;

    Rectangle bounds1 = obj1.getBounds();
    Rectangle bounds2 = obj2.getBounds();

    return checkRectangleCollision(bounds1, bounds2);
}

bool CollisionManager::checkRectangleCollision(const Rectangle &rect1, const Rectangle &rect2)
{
    return (rect1.x < rect2.x + rect2.width &&
            rect1.x + rect1.width > rect2.x &&
            rect1.y < rect2.y + rect2.height &&
            rect1.y + rect1.height > rect2.y);
}