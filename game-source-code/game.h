#ifndef GAME_H
#define GAME_H

#include <raylib-cpp.hpp>
#include <vector>

#include "menu.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"

/**
 *  @brief main game loop and initialize the window.
 *
 */
class Game
{
public:
    /**
     * @brief Default constructor for the Game class.
     */
    Game();
    /**
     * @brief Initializes the game resources and window.
     */
    void init();
    /**
     * @brief Updates the game state.
     */
    void update();
    /**
     * @brief Draws the game elements on the screen.
     */
    void draw();
    /**
     * @brief Runs the main game loop.
     */
    void run();

private:
    bool isRunning = true; // Indicates if the game is running
    // Screen dimensions
    int screenWidth = 900;
    int screenHeight = 700;
    raylib::Window window; // member variable for the window

    // Game states
    Menu menu;                // Instance of the Menu class to handle the game menu
    bool inMenu = true;       // Indicates if the game is in the menu state
    bool gameStarted = false; // Indicates if the game has started

    // Gameplay entities
    Player player;                 //!< Player spaceship
    std::vector<Bullet> bullets;   //!< Active bullets
    std::vector<Enemy> enemies;    //!< Active enemies
    float enemySpawnTimer = 0.0f;  //!< Timer for enemy spawning

    // methods for handling game states
    void updateMenu();
    void updateGame();
    void drawMenu();
    void drawGame();
};

#endif // GAME_H
