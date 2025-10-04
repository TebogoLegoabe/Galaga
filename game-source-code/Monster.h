#ifndef MONSTER_H
#define MONSTER_H

#include "GameObject.h"
#include "GameEnums.h"
#include "Grid.h"
#include "Player.h"
#include <raylib-cpp.hpp>
#include <functional>

/**
 * @brief Monster class for Dig Dug enemies
 */
class Monster : public GameObject
{
public:
    Monster(Vector2 startPos = {0, 0}, MonsterState state = MonsterState::IN_TUNNEL);

    void update() override;
    void draw() override;

    void updateAI(const Player &player, Grid &grid, bool canBecomeDisembodied,
                  std::function<void()> notifyDisembodied = nullptr);

    // Override canMoveTo for monster-specific movement rules
    bool canMoveTo(Vector2 newPos, const Grid &grid) const override;

    MonsterState getState() const;
    void setState(MonsterState newState);
    void reset(Vector2 startPos, MonsterState state = MonsterState::IN_TUNNEL);
    bool isDead() const;

protected:
    MonsterState currentState;
    float stateTimer;
    float aiUpdateTimer;
    Direction lastDirection;

    void updateStateTimer();
    bool shouldBecomeDisembodied(const Player &player, const Grid &grid);
    float calculateDistanceToPlayer(const Player &player) const;
    bool isPlayerInSameTunnel(const Player &player, const Grid &grid) const;

private:
    Direction findBestDirectionToPlayer(const Player &player, const Grid &grid);
    Direction findRandomValidDirection(const Grid &grid);
};

#endif // MONSTER_H