#ifndef BULLET_H
#define BULLET_H

#include <raylib-cpp.hpp>

/**
 * @brief Simple bullet fired by the player.
 */
class Bullet {
public:
    /**
     * @brief Construct a new Bullet at the given position.
     * @param pos Starting position for the bullet.
     */
    explicit Bullet(Vector2 pos);

    /**
     * @brief Update bullet position.
     */
    void update();

    /**
     * @brief Draw the bullet to the screen.
     */
    void draw() const;

    /**
     * @brief Check if bullet is outside the screen.
     */
    bool isOffScreen() const;

    /**
     * @brief Get rectangle for collision detection.
     */
    Rectangle getRect() const;

private:
    Vector2 position;      //!< Current bullet position
    float speed = -8.0f;   //!< Vertical speed (upwards)
    float width = 5.0f;    //!< Width of bullet
    float height = 10.0f;  //!< Height of bullet
};

#endif // BULLET_H
