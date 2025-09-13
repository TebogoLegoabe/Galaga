#ifndef GAME_H
#define GAME_H

#include <raylib-cpp.hpp>
#include "GameStateManager.h"

/**
 * @brief Main game class that manages the window and game loop
 */
class Game
{
public:
    /**
     * @brief Default constructor for the Game class
     */
    Game();

    /**
     * @brief Destructor
     */
    ~Game();

    /**
     * @brief Initializes the game resources and window
     */
    void init();

    /**
     * @brief Runs the main game loop
     */
    void run();

    /**
     * @brief Updates the game state
     */
    void update();

    /**
     * @brief Draws the game elements on the screen
     */
    void draw();

    /**
     * @brief Cleanup resources
     */
    void cleanup();

private:
    static const int SCREEN_WIDTH = 900;
    static const int SCREEN_HEIGHT = 700;

    bool isRunning;                // Indicates if the game is running
    raylib::Window window;         // Game window
    GameStateManager stateManager; // Manages game states

    /**
     * @brief Handle window events
     */
    void handleWindowEvents();
};

#endif // GAME_H