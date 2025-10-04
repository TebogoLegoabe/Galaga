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

class MonsterManager
{
public:
    MonsterManager();

    void initialize(const Level &level, Vector2 playerStartPos);
    void update(const Player &player, Grid &grid, bool canBecomeDisembodied,
                std::function<void()> notifyDisembodied);
    void draw();

    std::vector<std::unique_ptr<Monster>> &getMonsters();
    const std::vector<std::unique_ptr<Monster>> &getMonsters() const;
    bool areAllMonstersDead() const;
    void clear();

private:
    std::vector<std::unique_ptr<Monster>> monsters;

    // Initialization helpers
    void ensureMinimumSpawns(std::vector<Vector2> &spawnPositions, const Grid &grid, Vector2 playerStartPos);
    void createGreenDragons(const std::vector<Vector2> &spawnPositions, int count);
    void createRemainingMonsters(const std::vector<Vector2> &spawnPositions);
    void removeMonstersTooCloseToPlayer(Vector2 playerStartPos);
    void ensureMinimumGreenDragons(const Grid &grid, Vector2 playerStartPos, int minCount);
    void ensureMinimumMonsterCount(const Grid &grid, Vector2 playerStartPos, int minCount);

    // Utility functions
    template <typename T>
    int countMonsterType() const;
    std::vector<Vector2> findDistantTunnels(const Grid &grid, Vector2 playerStartPos, float minDistance);

    void addMonstersToEmptyTunnels(std::vector<Vector2> &spawnPositions, const Grid &grid, Vector2 playerStart);
    void addMonstersToDistantTunnels(const Grid &grid, Vector2 playerStart);
};

#endif // MONSTER_MANAGER_H