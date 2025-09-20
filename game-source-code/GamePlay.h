#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <raylib-cpp.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include "Level.h"
#include "Player.h"
#include "Monster.h"
#include "Harpoon.h"
#include "InputHandler.h"

/**
 * @brief Manages the main gameplay state
 */
class GamePlay
{
public:
    /**
     * @brief Constructor for GamePlay
     */
    GamePlay();

    /**
     * @brief Destructor
     */
    ~GamePlay();

    /**
     * @brief Initialize the gameplay
     */
    void init();

    /**
     * @brief Handle input for gameplay
     */
    void handleInput();

    /**
     * @brief Update the game state
     */
    void update();

    /**
     * @brief Draw the game
     */
    void draw();

    /**
     * @brief Check if the game is over
     * @return true if game is over
     */
    bool isGameOver() const;

    /**
     * @brief Check if the level is complete
     * @return true if level is complete
     */
    bool isLevelComplete() const;

    /**
     * @brief Check if player won
     * @return true if player won
     */
    bool didPlayerWin() const;

    /**
     * @brief Reset the gameplay to initial state
     */
    void reset();

    /**
     * @brief Get the player object for testing
     * @return Reference to the player
     */
    Player &getPlayer();

    /**
     * @brief Get the current level for testing
     * @return Reference to the current level
     */
    Level &getCurrentLevel();

    /**
     * @brief Get the monsters vector for testing
     * @return Reference to the monsters vector
     */
    std::vector<std::unique_ptr<Monster>> &getMonsters();

private:
    Level currentLevel;                             // The current level
    Player player;                                  // The player character
    std::vector<std::unique_ptr<Monster>> monsters; // The monsters
    bool gameOver;                                  // Game over flag
    bool levelComplete;                             // Level complete flag
    bool playerWon;                                 // Player won flag
    float disembodiedCooldown;                      // Timer to prevent multiple monsters becoming disembodied at once
    static const float DISEMBODIED_COOLDOWN_TIME;   // Cooldown duration between disembodied transitions

    void updateGameLogic();
    void drawHUD();
    void handlePlayerMovement();
    void initializeMonsters();
    void updateMonsters();
    void drawMonsters();
    void checkPlayerMonsterCollisions();
    void checkHarpoonMonsterCollisions();
    bool checkCollision(const GameObject &obj1, const GameObject &obj2) const;
    bool areAllMonstersDead() const;
    void addMonstersToEmptyTunnels(std::vector<Vector2> &spawnPositions);
    void addMonstersToDistantTunnels();
    bool canMonsterBecomeDisembodied() const;
    void notifyMonsterBecameDisembodied();
};

#endif // GAMEPLAY_H