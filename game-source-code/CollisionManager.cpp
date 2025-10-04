#include "CollisionManager.h"
#include "MonsterManager.h"
#include "GreenDragon.h"
#include "Fire.h"
#include "Rock.h"

bool CollisionManager::checkPlayerMonsterCollision(Player &player, MonsterManager &monsterManager)
{
    Rectangle playerBounds = player.getBounds();
    auto &monsters = monsterManager.getMonsters();

    for (const auto &monster : monsters)
    {
        if (monster && monster->isActive() && !monster->isDead())
        {
            Rectangle monsterBounds = monster->getBounds();
            if (CheckCollisionRecs(playerBounds, monsterBounds))
            {
                return true;
            }
        }
    }
    return false;
}

void CollisionManager::checkHarpoonMonsterCollisions(Player &player, MonsterManager &monsterManager,
                                                     const Grid &grid)
{
    Harpoon &harpoon = player.getHarpoon();
    if (!harpoon.isHarpoonActive())
        return;

    Rectangle harpoonBounds = harpoon.getBounds();
    auto &monsters = monsterManager.getMonsters();

    for (auto &monster : monsters)
    {
        if (monster && monster->isActive() && !monster->isDead())
        {
            Rectangle monsterBounds = monster->getBounds();
            if (CheckCollisionRecs(harpoonBounds, monsterBounds))
            {
                monster->setState(MonsterState::DEAD);
                harpoon.deactivate();
                return;
            }
        }
    }
}

bool CollisionManager::checkFirePlayerCollision(Player &player, MonsterManager &monsterManager)
{
    Rectangle playerBounds = player.getBounds();
    auto &monsters = monsterManager.getMonsters();

    for (auto &monster : monsters)
    {
        if (monster && monster->isActive() && !monster->isDead())
        {
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
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool CollisionManager::checkRockPlayerCollision(Player &player, const std::vector<std::unique_ptr<Rock>> &rocks)
{
    Rectangle playerBounds = player.getBounds();

    for (const auto &rock : rocks)
    {
        if (rock && rock->isActive() && rock->isFalling())
        {
            Rectangle rockBounds = rock->getBounds();
            if (CheckCollisionRecs(playerBounds, rockBounds))
            {
                return true;
            }
        }
    }
    return false;
}

void CollisionManager::checkRockMonsterCollisions(const std::vector<std::unique_ptr<Rock>> &rocks,
                                                  MonsterManager &monsterManager)
{
    auto &monsters = monsterManager.getMonsters();

    for (const auto &rock : rocks)
    {
        if (rock && rock->isActive() && rock->isFalling())
        {
            Rectangle rockBounds = rock->getBounds();

            for (auto &monster : monsters)
            {
                if (monster && monster->isActive() && !monster->isDead())
                {
                    Rectangle monsterBounds = monster->getBounds();
                    if (CheckCollisionRecs(rockBounds, monsterBounds))
                    {
                        monster->setState(MonsterState::DEAD);
                        // Rock continues falling after crushing monster
                    }
                }
            }
        }
    }
}

bool CollisionManager::checkCollision(const GameObject &obj1, const GameObject &obj2)
{
    Rectangle bounds1 = obj1.getBounds();
    Rectangle bounds2 = obj2.getBounds();
    return CheckCollisionRecs(bounds1, bounds2);
}