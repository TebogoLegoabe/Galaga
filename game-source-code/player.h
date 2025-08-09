#ifndef PLAYER_H
#define PLAYER_H

#include <raylib-cpp.hpp>
#include <vector>

#include "bullet.h"

/**
 * @brief Player controlled spaceship.
 */
class Player {
public:
    Player();
    ~Player();

    /**
     * @brief Handle movement and shooting.
     * @param bullets Container to append new bullets.
     */
    void update(std::vector<Bullet> &bullets);

    /**
     * @brief Draw the player sprite.
     */
    void draw() const;

    /**
     * @brief Bounding rectangle of the player.
     */
    Rectangle getRect() const;

private:
    Texture2D texture; //!< Spaceship texture
    Vector2 position;  //!< Current position
    float speed = 5.0f;
};

#endif // PLAYER_H
