#include <raylib-cpp.hpp>
#include <iostream>
#include "game.h"
/**
 *   * @brief Main function that initializes the game and starts the game loop.
 *   * @return int Returns 0 on successful execution.
 */
int main()
{
  // Object of the Game class
  Game game;
  // Run the game
  game.run();

  // Exit the program
  return 0;
}