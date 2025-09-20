#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <raylib-cpp.hpp>
#include <vector>
#include "Player.h"
#include "Monster.h"
#include "Harpoon.h"
#include "Grid.h"

class CollisionManager
{
public:
    static bool checkPlayerMonsterCollision(const Player &player,
                                            const std::vector<Monster *> &monsters);

    static Monster *checkHarpoonMonsterCollision(Harpoon &harpoon,
                                                 const std::vector<Monster *> &monsters,
                                                 const Grid &grid);

    static bool checkHarpoonBounds(const Harpoon &harpoon, const Grid &grid);

    static bool checkCollision(const GameObject &obj1, const GameObject &obj2);

    static bool checkRectangleCollision(const Rectangle &rect1, const Rectangle &rect2);

private:
    CollisionManager() = default;
};

#endif // COLLISION_MANAGER_H
