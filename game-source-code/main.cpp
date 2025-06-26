/**
 *  @file main.cpp
 *   @brief Main entry point for the game.
 *   @details This file initializes the game window and starts the game loop.
 *    @author Your Name: Tebogo Legoabe
 *    @date 2025-06-10
 */
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