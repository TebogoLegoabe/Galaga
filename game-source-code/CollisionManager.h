#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <vector>
#include <raylib-cpp.hpp>
#include "Monster.h"
#include "Player.h"
#include "Grid.h"

/**
 * @brief Manages collision detection between game objects
 */
class CollisionManager
{
public:
    /**
     * @brief Check collision between player and monsters
     * @param player Reference to the player
     * @param monsters Vector of monster pointers
     * @return Pointer to colliding monster, or nullptr if no collision
     */
    static Monster *checkPlayerMonsterCollision(const Player &player, const std::vector<std::unique_ptr<Monster>> &monsters);

    /**
     * @brief Check collision between harpoon and monsters
     * @param harpoon Reference to the harpoon
     * @param monsters Vector of monster pointers
     * @param grid Reference to the game grid
     * @return Pointer to hit monster, or nullptr if no collision
     */
    static Monster *checkHarpoonMonsterCollision(Harpoon &harpoon, const std::vector<std::unique_ptr<Monster>> &monsters, const Grid &grid);

    /**
     * @brief Check collision between harpoon and monsters (alternative signature)
     * @param harpoon Reference to the harpoon
     * @param monsters Vector of monster raw pointers
     * @param grid Reference to the game grid
     * @return Pointer to hit monster, or nullptr if no collision
     */
    static Monster *checkHarpoonMonsterCollision(Harpoon &harpoon, const std::vector<Monster *> &monsters, const Grid &grid);

    /**
     * @brief Check collision between player and monsters (alternative signature)
     * @param player Reference to the player
     * @param monsters Vector of monster raw pointers
     * @return Pointer to colliding monster, or nullptr if no collision
     */
    static Monster *checkPlayerMonsterCollision(const Player &player, const std::vector<Monster *> &monsters);

    /**
     * @brief Check if harpoon is out of bounds
     * @param harpoon Reference to the harpoon
     * @param grid Reference to the game grid
     * @return true if harpoon is out of bounds
     */
    static bool checkHarpoonBounds(const Harpoon &harpoon, const Grid &grid);
};

#endif // COLLISION_MANAGER_H