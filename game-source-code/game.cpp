#include "Game.h"
#include <iostream>

Game::Game()
    : isRunning(true),
      window(SCREEN_WIDTH, SCREEN_HEIGHT, "Dig Dug - Underground Adventure"),
      stateManager()
{
}

Game::~Game()
{
    cleanup();
}

void Game::init()
{
    // Set the exit key to none (we'll handle it manually)
    SetExitKey(0);

    // Set the target FPS
    window.SetTargetFPS(60);

    // Initialize the state manager
    stateManager.init();

    std::cout << "Game initialized successfully!" << std::endl;
}

void Game::run()
{
    init();

    while (isRunning && !WindowShouldClose())
    {
        handleWindowEvents();
        update();
        draw();
    }

    cleanup();
}

void Game::update()
{
    // Update the current game state
    stateManager.update();

    // Check if we should exit the game
    if (stateManager.shouldExit())
    {
        isRunning = false;
    }
}

void Game::draw()
{
    BeginDrawing();
    ClearBackground(DARKBROWN); // Earth-like background color

    // Draw the current state
    stateManager.draw();

    EndDrawing();
}

void Game::handleWindowEvents()
{
    // Handle any window-specific events here if needed
    if (WindowShouldClose())
    {
        isRunning = false;
    }
}

void Game::cleanup()
{
    // Any cleanup code will go here
    std::cout << "Game cleanup completed." << std::endl;
}