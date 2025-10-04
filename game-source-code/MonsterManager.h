#ifndef MONSTER_MANAGER_H
#define MONSTER_MANAGER_H

#include <vector>
#include <memory>
#include <functional>
#include "Monster.h"
#include "RedMonster.h"
#include "GreenDragon.h"
#include "Player.h"
#include "Grid.h"
#include "Level.h"

/**
 * @brief Manages all monster-related functionality
 */
class MonsterManager
{
public:
    /**
     * @brief Constructor
     */
    MonsterManager();

    /**
     * @brief Initialize monsters for a level
     * @param level Reference to the current level
     * @param playerStartPos Player's starting position
     */
    void initialize(const Level &level, Vector2 playerStartPos);

    /**
     * @brief Update all monsters
     * @param player Reference to the player
     * @param grid Reference to the game grid
     * @param canBecomeDisembodied Whether monsters can become disembodied
     * @param notifyDisembodied Callback when monster becomes disembodied
     */
    void update(const Player &player, Grid &grid, bool canBecomeDisembodied,
                std::function<void()> notifyDisembodied);

    /**
     * @brief Draw all monsters
     */
    void draw();

    /**
     * @brief Get the monsters vector
     * @return Reference to monsters vector
     */
    std::vector<std::unique_ptr<Monster>> &getMonsters();

    /**
     * @brief Get the monsters vector (const version)
     * @return Const reference to monsters vector
     */
    const std::vector<std::unique_ptr<Monster>> &getMonsters() const;

    /**
     * @brief Check if all monsters are dead
     * @return true if all monsters are dead
     */
    bool areAllMonstersDead() const;

    /**
     * @brief Clear all monsters
     */
    void clear();

private:
    std::vector<std::unique_ptr<Monster>> monsters;

    /**
     * @brief Add monsters to empty tunnels
     * @param spawnPositions Vector to add spawn positions to
     * @param grid Reference to the game grid
     * @param playerStart Player's starting position
     */
    void addMonstersToEmptyTunnels(std::vector<Vector2> &spawnPositions,
                                   const Grid &grid, Vector2 playerStart);

    /**
     * @brief Add monsters to distant tunnels
     * @param grid Reference to the game grid
     * @param playerStart Player's starting position
     */
    void addMonstersToDistantTunnels(const Grid &grid, Vector2 playerStart);
};

#endif // MONSTER_MANAGER_H