#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <vector>
#include <memory>
#include "Monster.h"
#include "Player.h"
#include "Harpoon.h"
#include "Grid.h"

/**
 * @brief Handles collision detection between game objects
 */
class CollisionManager
{
public:
    /**
     * @brief Check collision between player and monsters
     * @param player The player object
     * @param monsters Vector of monster unique pointers
     * @return Pointer to colliding monster, or nullptr if no collision
     */
    static Monster *checkPlayerMonsterCollision(const Player &player, const std::vector<std::unique_ptr<Monster>> &monsters);

    /**
     * @brief Check collision between harpoon and monsters
     * @param harpoon The harpoon object
     * @param monsters Vector of monster unique pointers
     * @param grid The game grid
     * @return Pointer to hit monster, or nullptr if no hit
     */
    static Monster *checkHarpoonMonsterCollision(Harpoon &harpoon, const std::vector<std::unique_ptr<Monster>> &monsters, const Grid &grid);

    /**
     * @brief Check collision between player and monsters (raw pointer version)
     * @param player The player object
     * @param monsters Vector of monster pointers
     * @return Pointer to colliding monster, or nullptr if no collision
     */
    static Monster *checkPlayerMonsterCollision(const Player &player, const std::vector<Monster *> &monsters);

    /**
     * @brief Check collision between harpoon and monsters (raw pointer version)
     * @param harpoon The harpoon object
     * @param monsters Vector of monster pointers
     * @param grid The game grid
     * @return Pointer to hit monster, or nullptr if no hit
     */
    static Monster *checkHarpoonMonsterCollision(Harpoon &harpoon, const std::vector<Monster *> &monsters, const Grid &grid);
};

#endif // COLLISION_MANAGER_H