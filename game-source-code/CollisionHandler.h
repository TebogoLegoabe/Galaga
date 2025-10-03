#ifndef COLLISION_HANDLER_H
#define COLLISION_HANDLER_H

#include "Player.h"
#include "MonsterManager.h"
#include "Grid.h"
#include "GreenDragon.h"
#include "Fire.h"

/**
 * @brief Handles all collision detection and resolution
 */
class CollisionHandler
{
public:
    /**
     * @brief Check and handle fire-player collisions
     * @param player Reference to the player
     * @param monsterManager Reference to the monster manager
     * @return true if player was hit by fire
     */
    static bool checkFirePlayerCollision(Player &player, MonsterManager &monsterManager);

    /**
     * @brief Check and handle player-monster collisions
     * @param player Reference to the player
     * @param monsterManager Reference to the monster manager
     * @return true if player was hit
     */
    static bool checkPlayerMonsterCollision(Player &player, MonsterManager &monsterManager);

    /**
     * @brief Check and handle harpoon-monster collisions
     * @param player Reference to the player (for harpoon access)
     * @param monsterManager Reference to the monster manager
     * @param grid Reference to the game grid
     */
    static void checkHarpoonMonsterCollisions(Player &player, MonsterManager &monsterManager,
                                              const Grid &grid);

private:
    /**
     * @brief Check collision between two game objects
     * @param obj1 First object
     * @param obj2 Second object
     * @return true if objects are colliding
     */
    static bool checkCollision(const GameObject &obj1, const GameObject &obj2);
};

#endif // COLLISION_HANDLER_H