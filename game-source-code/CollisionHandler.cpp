#include "CollisionHandler.h"

bool CollisionHandler::checkPlayerMonsterCollision(Player &player, MonsterManager &monsterManager)
{
    auto &monsters = monsterManager.getMonsters();

    for (auto &monster : monsters)
    {
        if (monster->isActive() && !monster->isDead())
        {
            if (checkCollision(player, *monster))
            {
                return true; // Player was hit
            }
        }
    }
    return false;
}

void CollisionHandler::checkHarpoonMonsterCollisions(Player &player, MonsterManager &monsterManager,
                                                     const Grid &grid)
{
    Harpoon &harpoon = player.getHarpoon();
    if (!harpoon.isHarpoonActive())
        return;

    Rectangle harpoonBounds = harpoon.getBounds();
    auto &monsters = monsterManager.getMonsters();

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

bool CollisionHandler::checkCollision(const GameObject &obj1, const GameObject &obj2)
{
    Rectangle bounds1 = obj1.getBounds();
    Rectangle bounds2 = obj2.getBounds();

    return CheckCollisionRecs(bounds1, bounds2);
}

bool CollisionHandler::checkFirePlayerCollision(Player &player, MonsterManager &monsterManager)
{
    auto &monsters = monsterManager.getMonsters();
    Rectangle playerBounds = player.getBounds();

    for (auto &monster : monsters)
    {
        if (monster->isActive() && !monster->isDead())
        {
            // Try to cast to GreenDragon
            GreenDragon *dragon = dynamic_cast<GreenDragon *>(monster.get());
            if (dragon)
            {
                Fire &fire = dragon->getFire();
                if (fire.isFireActive())
                {
                    Rectangle fireBounds = fire.getBounds();
                    if (CheckCollisionRecs(playerBounds, fireBounds))
                    {
                        fire.deactivate();
                        return true; // Player hit by fire
                    }
                }
            }
        }
    }
    return false;
}