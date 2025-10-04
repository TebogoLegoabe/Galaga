#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <vector>
#include <memory>
#include "Monster.h"
#include "Player.h"
#include "Harpoon.h"
#include "Grid.h"

// Forward declarations
class MonsterManager;
class GreenDragon;

/**
 * @brief Unified collision detection for all game objects
 */
class CollisionManager
{
public:
    /**
     * @brief Check collision between player and monsters
     * @param player The player object
     * @param monsterManager The monster manager
     * @return true if collision occurred
     */
    static bool checkPlayerMonsterCollision(Player &player, MonsterManager &monsterManager);

    /**
     * @brief Check collision between harpoon and monsters
     * @param player The player (for harpoon access)
     * @param monsterManager The monster manager
     * @param grid The game grid
     */
    static void checkHarpoonMonsterCollisions(Player &player, MonsterManager &monsterManager,
                                              const Grid &grid);

    /**
     * @brief Check collision between fire and player
     * @param player The player object
     * @param monsterManager The monster manager
     * @return true if player was hit by fire
     */
    static bool checkFirePlayerCollision(Player &player, MonsterManager &monsterManager);

    /**
     * @brief Check collision between rocks and player
     * @param player The player object
     * @param rocks Vector of rock objects
     * @return true if player was hit by rock
     */
    static bool checkRockPlayerCollision(Player &player, const std::vector<std::unique_ptr<class Rock>> &rocks);

    /**
     * @brief Check collision between rocks and monsters
     * @param rocks Vector of rock objects
     * @param monsterManager The monster manager
     */
    static void checkRockMonsterCollisions(const std::vector<std::unique_ptr<class Rock>> &rocks,
                                           MonsterManager &monsterManager);

private:
    /**
     * @brief Generic collision check between two game objects
     * @param obj1 First object
     * @param obj2 Second object
     * @return true if objects collide
     */
    static bool checkCollision(const class GameObject &obj1, const class GameObject &obj2);
};

#endif // COLLISION_MANAGER_H