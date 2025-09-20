#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <raylib-cpp.hpp>
#include <vector>
#include "Player.h"
#include "Monster.h"

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
     * @return true if collision detected
     */
    static bool checkPlayerMonsterCollision(const Player &player,
                                            const std::vector<Monster *> &monsters);

    /**
     * @brief Check collision between two game objects
     * @param obj1 First object
     * @param obj2 Second object
     * @return true if collision detected
     */
    static bool checkCollision(const GameObject &obj1, const GameObject &obj2);

    /**
     * @brief Check if two rectangles overlap
     * @param rect1 First rectangle
     * @param rect2 Second rectangle
     * @return true if rectangles overlap
     */
    static bool checkRectangleCollision(const Rectangle &rect1, const Rectangle &rect2);

private:
    // Static class - no instances needed
    CollisionManager() = default;
};

#endif // COLLISION_MANAGER_H