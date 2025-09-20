#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "GameEnums.h"
#include "Grid.h"
#include "Sprite.h"
#include "Harpoon.h"
#include <memory>
#include <raylib-cpp.hpp>

class Player : public GameObject
{
public:
    Player(Vector2 startPos = {0, 0});
    void update() override;
    void draw() override;
    bool move(Direction direction, Grid &grid);
    bool shootHarpoon();
    Direction getFacingDirection() const;
    Harpoon &getHarpoon();
    const Harpoon &getHarpoon() const;
    bool canMoveTo(Vector2 newPos, const Grid &grid) const;
    Vector2 getGridPosition(const Grid &grid) const;
    void reset(Vector2 startPos);
    float getSpeed() const;
    void setSpeed(float newSpeed);
    bool canShoot() const;

private:
    Direction facingDirection;
    float speed;
    Vector2 targetPosition;
    bool isMoving;
    Harpoon harpoon;

    void updateMovement();
    void digAtCurrentPosition(Grid &grid);
    bool isWithinGridBounds(Vector2 worldPos, const Grid &grid) const;
    Vector2 getHarpoonStartPosition() const;
};

#endif // PLAYER_H