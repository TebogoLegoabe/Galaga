#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <raylib-cpp.hpp>
#include <vector>
#include <memory>
#include "Level.h"
#include "Player.h"
#include "Monster.h"
#include "RedMonster.h"
#include "CollisionManager.h"
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
     * @brief Get the monsters for testing
     * @return Reference to the monsters vector
     */
    const std::vector<std::unique_ptr<Monster>> &getMonsters() const;

private:
    Level currentLevel;                             ///< The current level
    Player player;                                  ///< The player character
    std::vector<std::unique_ptr<Monster>> monsters; ///< Game monsters
    bool gameOver;                                  ///< Game over flag
    bool levelComplete;                             ///< Level complete flag
    bool playerWon;                                 ///< Player won flag

    /**
     * @brief Update game logic
     */
    void updateGameLogic();

    /**
     * @brief Draw the HUD (heads-up display)
     */
    void drawHUD();

    /**
     * @brief Handle player movement input
     */
    void handlePlayerMovement();

    /**
     * @brief Initialize monsters at spawn positions
     */
    void initializeMonsters();

    /**
     * @brief Update all monsters
     */
    void updateMonsters();

    /**
     * @brief Draw all monsters
     */
    void drawMonsters();

    /**
     * @brief Check win condition (all monsters defeated)
     * @return true if all monsters are dead
     */
    bool checkWinCondition() const;
};

#endif // GAMEPLAY_H