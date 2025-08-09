#ifndef ENEMY_H
#define ENEMY_H

#include <raylib-cpp.hpp>

/**
 * @brief Simple enemy descending from top of the screen.
 */
class Enemy {
public:
    /**
     * @brief Construct a new Enemy at given position.
     */
    explicit Enemy(Vector2 pos);

    /**
     * @brief Update enemy position.
     */
    void update();

    /**
     * @brief Draw enemy to screen.
     */
    void draw() const;

    /**
     * @brief Check if enemy is outside the screen.
     */
    bool isOffScreen() const;

    /**
     * @brief Get rectangle for collision detection.
     */
    Rectangle getRect() const;

private:
    Vector2 position;    //!< Current position
    float speed = 2.0f;  //!< Downward speed
    float size = 20.0f;  //!< Square size of enemy
};

#endif // ENEMY_H
