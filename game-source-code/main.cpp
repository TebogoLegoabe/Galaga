#include <raylib-cpp.hpp>
#include <iostream>
#include "game.h"
/**
 *   * @brief Main function that initializes the game and starts the game loop.
 *   * @return int Returns 0 on successful execution.
 */
int main()
{
  // object of the Game class
  Game game;
  // run the game
  game.run();

  // Exit the program
  return 0;
}