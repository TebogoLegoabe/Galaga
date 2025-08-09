#include "game.h"
#include <algorithm>
#include <iostream>

Game::Game() : isRunning(true), screenWidth(900), screenHeight(700),
               window(screenWidth, screenHeight, "Game Window"), menu(), inMenu(true),
               gameStarted(false) {}

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
        return; // Exit the game loop
    }
    // Update game state based on the current menu state
    if (inMenu)
    {
        updateMenu();
    }
    else
    {
        updateGame();
    }
}

void Game::updateMenu()
{
    // Handle menu input
    menu.handleInput();

    // Check if the game should start or exit
    if (menu.shouldStartGame())
    {
        inMenu = false;     // Switch to game state
        gameStarted = true; // Game has started
    }
    else if (menu.shouldExitGame())
    {
        isRunning = false; // Exit the game
    }
}
void Game::updateGame()
{
    // press ESC to return to the menu
    if (IsKeyPressed(KEY_ESCAPE))
    {
        inMenu = true;       // Switch back to menu state
        gameStarted = false; // Game has not started
        menu.reset();        // Reset the menu state
        bullets.clear();
        enemies.clear();
        return;
    }

    // update player and spawn bullets
    player.update(bullets);

    // update bullets
    for (auto &b : bullets)
        b.update();
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [](const Bullet &b)
                                 { return b.isOffScreen(); }),
                  bullets.end());

    // spawn enemies
    enemySpawnTimer += GetFrameTime();
    if (enemySpawnTimer > 1.0f)
    {
        float x = static_cast<float>(GetRandomValue(0, screenWidth - 20));
        enemies.emplace_back(Vector2{x, -20.0f});
        enemySpawnTimer = 0.0f;
    }

    // update enemies
    for (auto &e : enemies)
        e.update();
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                 [](const Enemy &e)
                                 { return e.isOffScreen(); }),
                  enemies.end());

    // handle bullet-enemy collisions
    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end();)
    {
        bool removedBullet = false;
        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();)
        {
            if (CheckCollisionRecs(bulletIt->getRect(), enemyIt->getRect()))
            {
                enemyIt = enemies.erase(enemyIt);
                bulletIt = bullets.erase(bulletIt);
                removedBullet = true;
                break;
            }
            else
            {
                ++enemyIt;
            }
        }
        if (!removedBullet)
            ++bulletIt;
    }
}
void Game::draw()
{
    // Begin drawing
    BeginDrawing();

    if (inMenu)
    {
        drawMenu(); // Draw the menu
    }
    else
    {
        drawGame(); // Draw the game
    }
    // End drawing
    EndDrawing();
}

void Game::drawMenu()
{
    // Draw the menu
    menu.draw();
}

void Game::drawGame()
{
    // Draw the game elements
    ClearBackground(BLUE); // Clear the screen with a blue background

    // For now, we will just draw a simple message
    const char *message = "Game is running... Press ESC to return to menu";
    int textWidth = MeasureText(message, 20);
    DrawText(message, (screenWidth - textWidth) / 2, screenHeight / 2, 20, DARKGRAY);
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
