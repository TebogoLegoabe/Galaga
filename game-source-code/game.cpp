#include "game.h"
#include <iostream>

Game::Game() : isRunning(true), screenWidth(900), screenHeight(700),
               window(screenWidth, screenHeight, "Defender Game") {}

void Game::init()
{
    // Set the exit key to ESC
    SetExitKey(KEY_ESCAPE);

    // Set the target FPS (frames per second)
    window.SetTargetFPS(60);
}

void Game::update()
{
    // Check for exit condition
    if (WindowShouldClose())
    {
        isRunning = false;
    }
    // Update game logic
}

void Game::draw()
{
    // Begin drawing
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Draw game elements

    // End drawing
    EndDrawing();
}

void Game::run()
{
    init(); // Initialize the game

    while (isRunning)
    {
        update(); // Update game  state
        draw();   // Draw the game
    }

    // Close the game window
    CloseWindow();
}