#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

#include "GameEnums.h"
#include <memory>

// Forward declarations
class Menu;
class GamePlay;

/**
 * @brief Manages different game states
 */
class GameStateManager
{
public:
    /**
     * @brief Constructor
     */
    GameStateManager();

    /**
     * @brief Destructor
     */
    ~GameStateManager();

    /**
     * @brief Initialize the state manager
     */
    void init();

    /**
     * @brief Update the current state
     */
    void update();

    /**
     * @brief Draw the current state
     */
    void draw();

    /**
     * @brief Switch to a new game state
     * @param newState The new state to switch to
     */
    void switchState(GameState newState);

    /**
     * @brief Get the current game state
     * @return Current GameState
     */
    GameState getCurrentState() const;

    /**
     * @brief Check if the game should exit
     * @return true if should exit, false otherwise
     */
    bool shouldExit() const;

private:
    GameState currentState;
    std::unique_ptr<Menu> menuState;
    std::unique_ptr<GamePlay> gamePlayState;
    bool exitRequested;
};

#endif // GAME_STATE_MANAGER_H