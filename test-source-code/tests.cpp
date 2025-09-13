// ========== tests.cpp (Updated) ==========
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <raylib-cpp.hpp>

// Include headers for classes we want to test
#include "Menu.h"
#include "GameStateManager.h"
#include "Grid.h"
#include "Level.h"
#include "GamePlay.h"

TEST_CASE("Menu initializes in main menu state")
{
    // Arrange & Act
    Menu menu;

    // Assert
    CHECK(menu.getMenuState() == MenuState::MAIN_MENU);
}

TEST_CASE("Menu does not start game initially")
{
    // Arrange & Act
    Menu menu;

    // Assert
    CHECK(menu.shouldStartGame() == false);
}

TEST_CASE("Menu does not exit game initially")
{
    // Arrange & Act
    Menu menu;

    // Assert
    CHECK(menu.shouldExitGame() == false);
}

TEST_CASE("Menu changes to game over state when set to game over")
{
    // Arrange
    Menu menu;

    // Act
    menu.setGameOver(true);

    // Assert
    CHECK(menu.getMenuState() == MenuState::GAME_OVER);
}

TEST_CASE("Menu changes to level complete state when set to level complete")
{
    // Arrange
    Menu menu;

    // Act
    menu.setLevelComplete();

    // Assert
    CHECK(menu.getMenuState() == MenuState::LEVEL_COMPLETE);
}

TEST_CASE("Menu returns to main menu state when reset")
{
    // Arrange
    Menu menu;
    menu.setGameOver(true);

    // Act
    menu.reset();

    // Assert
    CHECK(menu.getMenuState() == MenuState::MAIN_MENU);
}

TEST_CASE("GameStateManager initializes in menu state")
{
    // Arrange & Act
    GameStateManager stateManager;

    // Assert
    CHECK(stateManager.getCurrentState() == GameState::MENU);
}

TEST_CASE("GameStateManager does not request exit initially")
{
    // Arrange & Act
    GameStateManager stateManager;

    // Assert
    CHECK(stateManager.shouldExit() == false);
}

TEST_CASE("GameStateManager switches to playing state")
{
    // Arrange
    GameStateManager stateManager;

    // Act
    stateManager.switchState(GameState::PLAYING);

    // Assert
    CHECK(stateManager.getCurrentState() == GameState::PLAYING);
}

TEST_CASE("GameStateManager switches to game over state")
{
    // Arrange
    GameStateManager stateManager;

    // Act
    stateManager.switchState(GameState::GAME_OVER);

    // Assert
    CHECK(stateManager.getCurrentState() == GameState::GAME_OVER);
}

TEST_CASE("Grid initializes with correct dimensions")
{
    // Arrange & Act
    Grid grid(20, 15, 32);

    // Assert
    CHECK(grid.getWidth() == 20);
    CHECK(grid.getHeight() == 15);
    CHECK(grid.getTileSize() == 32);
}

TEST_CASE("Grid initializes with all earth tiles")
{
    // Arrange & Act
    Grid grid(5, 5, 32);

    // Assert
    CHECK(grid.getTile(0, 0) == TileType::EARTH);
    CHECK(grid.getTile(2, 2) == TileType::EARTH);
    CHECK(grid.getTile(4, 4) == TileType::EARTH);
}

TEST_CASE("Grid validates position boundaries correctly")
{
    // Arrange
    Grid grid(10, 10, 32);

    // Act & Assert
    CHECK(grid.isValidPosition(0, 0) == true);
    CHECK(grid.isValidPosition(9, 9) == true);
    CHECK(grid.isValidPosition(10, 5) == false);
    CHECK(grid.isValidPosition(5, 10) == false);
    CHECK(grid.isValidPosition(-1, 5) == false);
}

TEST_CASE("Grid sets tile type correctly")
{
    // Arrange
    Grid grid(10, 10, 32);

    // Act
    grid.setTile(5, 5, TileType::TUNNEL);

    // Assert
    CHECK(grid.getTile(5, 5) == TileType::TUNNEL);
}

TEST_CASE("Grid digs tunnel only in earth")
{
    // Arrange
    Grid grid(10, 10, 32);
    grid.setTile(3, 3, TileType::ROCK);

    // Act
    grid.digTunnel(2, 2); // Should work (earth -> tunnel)
    grid.digTunnel(3, 3); // Should not work (rock stays rock)

    // Assert
    CHECK(grid.getTile(2, 2) == TileType::TUNNEL);
    CHECK(grid.getTile(3, 3) == TileType::ROCK);
}

TEST_CASE("Grid identifies tunnels correctly")
{
    // Arrange
    Grid grid(10, 10, 32);
    grid.setTile(4, 4, TileType::TUNNEL);

    // Act & Assert
    CHECK(grid.isTunnel(4, 4) == true);
    CHECK(grid.isTunnel(5, 5) == false);
}

TEST_CASE("Grid converts world to grid coordinates correctly")
{
    // Arrange
    Grid grid(10, 10, 32);
    Vector2 worldPos = {96.0f, 64.0f}; // Should be grid position (3, 2)

    // Act
    Vector2 gridPos = grid.worldToGrid(worldPos);

    // Assert
    CHECK(gridPos.x == 3.0f);
    CHECK(gridPos.y == 2.0f);
}

TEST_CASE("Grid converts grid to world coordinates correctly")
{
    // Arrange
    Grid grid(10, 10, 32);

    // Act
    Vector2 worldPos = grid.gridToWorld(3, 2);

    // Assert
    CHECK(worldPos.x == 96.0f);
    CHECK(worldPos.y == 64.0f);
}

TEST_CASE("Level initializes with valid player start position")
{
    // Arrange & Act
    Level level;
    Vector2 playerStart = level.getPlayerStartPosition();

    // Assert
    CHECK(playerStart.x >= 0);
    CHECK(playerStart.y >= 0);
    CHECK(level.isWithinBounds(playerStart) == true);
}

TEST_CASE("Level has monster spawn positions")
{
    // Arrange & Act
    Level level;
    std::vector<Vector2> spawns = level.getMonsterSpawnPositions();

    // Assert
    CHECK(spawns.size() > 0);
}

TEST_CASE("Level has rock positions")
{
    // Arrange & Act
    Level level;
    std::vector<Vector2> rocks = level.getRockPositions();

    // Assert
    CHECK(rocks.size() > 0);
}

TEST_CASE("Level checks world position bounds correctly")
{
    // Arrange
    Level level;
    Vector2 validPos = {100, 100};
    Vector2 invalidPos = {-10, -10};

    // Act & Assert
    CHECK(level.isWithinBounds(validPos) == true);
    CHECK(level.isWithinBounds(invalidPos) == false);
}

// ========== tests.cpp (Updated) ==========
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <raylib-cpp.hpp>

// Include headers for classes we want to test
#include "Menu.h"
#include "GameStateManager.h"
#include "Grid.h"
#include "Level.h"
#include "GamePlay.h"

TEST_CASE("Menu initializes in main menu state")
{
    // Arrange & Act
    Menu menu;

    // Assert
    CHECK(menu.getMenuState() == MenuState::MAIN_MENU);
}

TEST_CASE("Menu does not start game initially")
{
    // Arrange & Act
    Menu menu;

    // Assert
    CHECK(menu.shouldStartGame() == false);
}

TEST_CASE("Menu does not exit game initially")
{
    // Arrange & Act
    Menu menu;

    // Assert
    CHECK(menu.shouldExitGame() == false);
}

TEST_CASE("Menu changes to game over state when set to game over")
{
    // Arrange
    Menu menu;

    // Act
    menu.setGameOver(true);

    // Assert
    CHECK(menu.getMenuState() == MenuState::GAME_OVER);
}

TEST_CASE("Menu changes to level complete state when set to level complete")
{
    // Arrange
    Menu menu;

    // Act
    menu.setLevelComplete();

    // Assert
    CHECK(menu.getMenuState() == MenuState::LEVEL_COMPLETE);
}

TEST_CASE("Menu returns to main menu state when reset")
{
    // Arrange
    Menu menu;
    menu.setGameOver(true);

    // Act
    menu.reset();

    // Assert
    CHECK(menu.getMenuState() == MenuState::MAIN_MENU);
}

TEST_CASE("GameStateManager initializes in menu state")
{
    // Arrange & Act
    GameStateManager stateManager;

    // Assert
    CHECK(stateManager.getCurrentState() == GameState::MENU);
}

TEST_CASE("GameStateManager does not request exit initially")
{
    // Arrange & Act
    GameStateManager stateManager;

    // Assert
    CHECK(stateManager.shouldExit() == false);
}

TEST_CASE("GameStateManager switches to playing state")
{
    // Arrange
    GameStateManager stateManager;

    // Act
    stateManager.switchState(GameState::PLAYING);

    // Assert
    CHECK(stateManager.getCurrentState() == GameState::PLAYING);
}

TEST_CASE("GameStateManager switches to game over state")
{
    // Arrange
    GameStateManager stateManager;

    // Act
    stateManager.switchState(GameState::GAME_OVER);

    // Assert
    CHECK(stateManager.getCurrentState() == GameState::GAME_OVER);
}

TEST_CASE("Grid initializes with correct dimensions")
{
    // Arrange & Act
    Grid grid(20, 15, 32);

    // Assert
    CHECK(grid.getWidth() == 20);
    CHECK(grid.getHeight() == 15);
    CHECK(grid.getTileSize() == 32);
}

TEST_CASE("Grid initializes with all earth tiles")
{
    // Arrange & Act
    Grid grid(5, 5, 32);

    // Assert
    CHECK(grid.getTile(0, 0) == TileType::EARTH);
    CHECK(grid.getTile(2, 2) == TileType::EARTH);
    CHECK(grid.getTile(4, 4) == TileType::EARTH);
}

TEST_CASE("Grid validates position boundaries correctly")
{
    // Arrange
    Grid grid(10, 10, 32);

    // Act & Assert
    CHECK(grid.isValidPosition(0, 0) == true);
    CHECK(grid.isValidPosition(9, 9) == true);
    CHECK(grid.isValidPosition(10, 5) == false);
    CHECK(grid.isValidPosition(5, 10) == false);
    CHECK(grid.isValidPosition(-1, 5) == false);
}

TEST_CASE("Grid sets tile type correctly")
{
    // Arrange
    Grid grid(10, 10, 32);

    // Act
    grid.setTile(5, 5, TileType::TUNNEL);

    // Assert
    CHECK(grid.getTile(5, 5) == TileType::TUNNEL);
}

TEST_CASE("Grid digs tunnel only in earth")
{
    // Arrange
    Grid grid(10, 10, 32);
    grid.setTile(3, 3, TileType::ROCK);

    // Act
    grid.digTunnel(2, 2); // Should work (earth -> tunnel)
    grid.digTunnel(3, 3); // Should not work (rock stays rock)

    // Assert
    CHECK(grid.getTile(2, 2) == TileType::TUNNEL);
    CHECK(grid.getTile(3, 3) == TileType::ROCK);
}

TEST_CASE("Grid identifies tunnels correctly")
{
    // Arrange
    Grid grid(10, 10, 32);
    grid.setTile(4, 4, TileType::TUNNEL);

    // Act & Assert
    CHECK(grid.isTunnel(4, 4) == true);
    CHECK(grid.isTunnel(5, 5) == false);
}

TEST_CASE("Grid converts world to grid coordinates correctly")
{
    // Arrange
    Grid grid(10, 10, 32);
    Vector2 worldPos = {96.0f, 64.0f}; // Should be grid position (3, 2)

    // Act
    Vector2 gridPos = grid.worldToGrid(worldPos);

    // Assert
    CHECK(gridPos.x == 3.0f);
    CHECK(gridPos.y == 2.0f);
}

TEST_CASE("Grid converts grid to world coordinates correctly")
{
    // Arrange
    Grid grid(10, 10, 32);

    // Act
    Vector2 worldPos = grid.gridToWorld(3, 2);

    // Assert
    CHECK(worldPos.x == 96.0f);
    CHECK(worldPos.y == 64.0f);
}

TEST_CASE("Level initializes with valid player start position")
{
    // Arrange & Act
    Level level;
    Vector2 playerStart = level.getPlayerStartPosition();

    // Assert
    CHECK(playerStart.x >= 0);
    CHECK(playerStart.y >= 0);
    CHECK(level.isWithinBounds(playerStart) == true);
}

TEST_CASE("Level has monster spawn positions")
{
    // Arrange & Act
    Level level;
    std::vector<Vector2> spawns = level.getMonsterSpawnPositions();

    // Assert
    CHECK(spawns.size() > 0);
}

TEST_CASE("Level has rock positions")
{
    // Arrange & Act
    Level level;
    std::vector<Vector2> rocks = level.getRockPositions();

    // Assert
    CHECK(rocks.size() > 0);
}

TEST_CASE("Level checks world position bounds correctly")
{
    // Arrange
    Level level;
    Vector2 validPos = {100, 100};
    Vector2 invalidPos = {-10, -10};

    // Act & Assert
    CHECK(level.isWithinBounds(validPos) == true);
    CHECK(level.isWithinBounds(invalidPos) == false);
}