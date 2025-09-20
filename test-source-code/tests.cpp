#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <raylib-cpp.hpp>

// Include headers for classes we want to test
#include "Menu.h"
#include "GameStateManager.h"
#include "Grid.h"
#include "Level.h"
#include "GamePlay.h"
#include "Player.h"
#include "RedMonster.h"
#include "CollisionManager.h"

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

TEST_CASE("GamePlay initializes without game over")
{
    // Arrange & Act
    GamePlay gameplay;

    // Assert
    CHECK(gameplay.isGameOver() == false);
    CHECK(gameplay.isLevelComplete() == false);
    CHECK(gameplay.didPlayerWin() == false);
}

TEST_CASE("GamePlay resets to initial state")
{
    // Arrange
    GamePlay gameplay;
    gameplay.init();

    // Act
    gameplay.reset();

    // Assert
    CHECK(gameplay.isGameOver() == false);
    CHECK(gameplay.isLevelComplete() == false);
    CHECK(gameplay.didPlayerWin() == false);
}

TEST_CASE("Player initializes with correct starting position")
{
    // Arrange
    Vector2 startPos = {100, 200};

    // Act
    Player player(startPos);

    // Assert
    CHECK(player.getPosition().x == 100.0f);
    CHECK(player.getPosition().y == 200.0f);
    CHECK(player.isActive() == true);
}

TEST_CASE("Player starts facing right by default")
{
    // Arrange & Act
    Player player({0, 0});

    // Assert
    CHECK(player.getFacingDirection() == Direction::RIGHT);
}

TEST_CASE("Player has default speed")
{
    // Arrange & Act
    Player player({0, 0});

    // Assert
    CHECK(player.getSpeed() == 2.0f);
}

TEST_CASE("Player speed can be changed")
{
    // Arrange
    Player player({0, 0});
    float newSpeed = 5.0f;

    // Act
    player.setSpeed(newSpeed);

    // Assert
    CHECK(player.getSpeed() == 5.0f);
}

TEST_CASE("Player can move right in grid")
{
    // Arrange
    Player player({32, 32}); // Start at grid position (1, 1)
    Grid grid(10, 10, 32);

    // Act
    bool moved = player.move(Direction::RIGHT, grid);

    // Assert
    CHECK(moved == true);
    CHECK(player.getFacingDirection() == Direction::RIGHT);
}

TEST_CASE("Player can move left in grid")
{
    // Arrange
    Player player({64, 32}); // Start at grid position (2, 1)
    Grid grid(10, 10, 32);

    // Act
    bool moved = player.move(Direction::LEFT, grid);

    // Assert
    CHECK(moved == true);
    CHECK(player.getFacingDirection() == Direction::LEFT);
}

TEST_CASE("Player can move up in grid")
{
    // Arrange
    Player player({32, 64}); // Start at grid position (1, 2)
    Grid grid(10, 10, 32);

    // Act
    bool moved = player.move(Direction::UP, grid);

    // Assert
    CHECK(moved == true);
    CHECK(player.getFacingDirection() == Direction::UP);
}

TEST_CASE("Player can move down in grid")
{
    // Arrange
    Player player({32, 32}); // Start at grid position (1, 1)
    Grid grid(10, 10, 32);

    // Act
    bool moved = player.move(Direction::DOWN, grid);

    // Assert
    CHECK(moved == true);
    CHECK(player.getFacingDirection() == Direction::DOWN);
}

TEST_CASE("Player cannot move outside grid boundaries")
{
    // Arrange
    Player player({0, 0}); // Start at edge
    Grid grid(10, 10, 32);

    // Act
    bool moved = player.move(Direction::LEFT, grid);

    // Assert
    CHECK(moved == false);
}

TEST_CASE("Player cannot move into rock tiles")
{
    // Arrange
    Player player({32, 32}); // Start at grid position (1, 1)
    Grid grid(10, 10, 32);
    grid.setTile(2, 1, TileType::ROCK); // Place rock to the right

    // Act
    bool moved = player.move(Direction::RIGHT, grid);

    // Assert
    CHECK(moved == false);
}

TEST_CASE("Player can move into earth tiles")
{
    // Arrange
    Player player({32, 32}); // Start at grid position (1, 1)
    Grid grid(10, 10, 32);
    // Earth tile at (2, 1) by default

    // Act
    bool moved = player.move(Direction::RIGHT, grid);

    // Assert
    CHECK(moved == true);
}

TEST_CASE("Player can move into tunnel tiles")
{
    // Arrange
    Player player({32, 32}); // Start at grid position (1, 1)
    Grid grid(10, 10, 32);
    grid.setTile(2, 1, TileType::TUNNEL); // Create tunnel to the right

    // Act
    bool moved = player.move(Direction::RIGHT, grid);

    // Assert
    CHECK(moved == true);
}

TEST_CASE("Player digs tunnel when moving through earth")
{
    // Arrange
    Player player({32, 32}); // Start at grid position (1, 1)
    Grid grid(10, 10, 32);
    // Position (2, 1) starts as earth

    // Act
    player.move(Direction::RIGHT, grid);

    // Assert
    CHECK(grid.getTile(2, 1) == TileType::TUNNEL);
}

TEST_CASE("Player calculates correct grid position")
{
    // Arrange
    Player player({64, 96}); // World position
    Grid grid(10, 10, 32);

    // Act
    Vector2 gridPos = player.getGridPosition(grid);

    // Assert
    CHECK(gridPos.x == 2.0f);
    CHECK(gridPos.y == 3.0f);
}

TEST_CASE("Player resets to starting position correctly")
{
    // Arrange
    Player player({100, 100});
    Vector2 newStart = {200, 300};

    // Act
    player.reset(newStart);

    // Assert
    CHECK(player.getPosition().x == 200.0f);
    CHECK(player.getPosition().y == 300.0f);
    CHECK(player.getFacingDirection() == Direction::RIGHT);
    CHECK(player.isActive() == true);
}

TEST_CASE("Player validates positions within grid bounds")
{
    // Arrange
    Player player({0, 0});
    Grid grid(10, 10, 32);
    Vector2 validPos = {32, 32};
    Vector2 invalidPos = {-10, -10};

    // Act & Assert
    CHECK(player.canMoveTo(validPos, grid) == true);
    CHECK(player.canMoveTo(invalidPos, grid) == false);
}

TEST_CASE("RedMonster initializes with correct starting position")
{
    // Arrange
    Vector2 startPos = {64, 96};

    // Act
    RedMonster monster(startPos);

    // Assert
    CHECK(monster.getPosition().x == 64.0f);
    CHECK(monster.getPosition().y == 96.0f);
    CHECK(monster.isActive() == true);
}

TEST_CASE("RedMonster starts in tunnel state")
{
    // Arrange & Act
    RedMonster monster({0, 0});

    // Assert
    CHECK(monster.getState() == MonsterState::IN_TUNNEL);
}

TEST_CASE("RedMonster can move in valid tunnel")
{
    // Arrange
    RedMonster monster({32, 32}); // Grid position (1, 1)
    Grid grid(10, 10, 32);
    grid.setTile(2, 1, TileType::TUNNEL); // Make right position a tunnel

    // Act
    bool moved = monster.move(Direction::RIGHT, grid);

    // Assert
    CHECK(moved == true);
}

TEST_CASE("RedMonster cannot move into earth")
{
    // Arrange
    RedMonster monster({32, 32}); // Grid position (1, 1)
    Grid grid(10, 10, 32);
    // Position (2, 1) is earth by default

    // Act
    bool moved = monster.move(Direction::RIGHT, grid);

    // Assert
    CHECK(moved == false);
}

TEST_CASE("RedMonster cannot move into rock")
{
    // Arrange
    RedMonster monster({32, 32}); // Grid position (1, 1)
    Grid grid(10, 10, 32);
    grid.setTile(2, 1, TileType::ROCK); // Place rock to the right

    // Act
    bool moved = monster.move(Direction::RIGHT, grid);

    // Assert
    CHECK(moved == false);
}

TEST_CASE("RedMonster calculates correct grid position")
{
    // Arrange
    RedMonster monster({96, 128}); // World position
    Grid grid(10, 10, 32);

    // Act
    Vector2 gridPos = monster.getGridPosition(grid);

    // Assert
    CHECK(gridPos.x == 3.0f);
    CHECK(gridPos.y == 4.0f);
}

TEST_CASE("RedMonster resets to starting position correctly")
{
    // Arrange
    RedMonster monster({100, 100});
    Vector2 newStart = {200, 300};

    // Act
    monster.reset(newStart);

    // Assert
    CHECK(monster.getPosition().x == 200.0f);
    CHECK(monster.getPosition().y == 300.0f);
    CHECK(monster.getState() == MonsterState::IN_TUNNEL);
    CHECK(monster.isActive() == true);
}

TEST_CASE("CollisionManager detects player-monster collision")
{
    // Arrange
    Player player({32, 32});
    RedMonster monster({32, 32}); // Same position
    std::vector<Monster *> monsters = {&monster};

    // Act
    bool collision = CollisionManager::checkPlayerMonsterCollision(player, monsters);

    // Assert
    CHECK(collision == true);
}

TEST_CASE("CollisionManager detects no collision when objects apart")
{
    // Arrange
    Player player({32, 32});
    RedMonster monster({96, 96}); // Different position
    std::vector<Monster *> monsters = {&monster};

    // Act
    bool collision = CollisionManager::checkPlayerMonsterCollision(player, monsters);

    // Assert
    CHECK(collision == false);
}

TEST_CASE("CollisionManager ignores dead monsters")
{
    // Arrange
    Player player({32, 32});
    RedMonster monster({32, 32});         // Same position
    monster.setState(MonsterState::DEAD); // Monster is dead
    std::vector<Monster *> monsters = {&monster};

    // Act
    bool collision = CollisionManager::checkPlayerMonsterCollision(player, monsters);

    // Assert
    CHECK(collision == false);
}

TEST_CASE("GamePlay initializes with monsters")
{
    // Arrange & Act
    GamePlay gameplay;
    gameplay.init();

    // Assert
    CHECK(gameplay.getMonsters().size() > 0);
}

TEST_CASE("GamePlay win condition works when no monsters alive")
{
    // Arrange
    GamePlay gameplay;
    gameplay.init();

    // Act - Kill all monsters
    for (const auto &monster : gameplay.getMonsters())
    {
        monster->setState(MonsterState::DEAD);
    }
    gameplay.update(); // This should trigger win condition

    // Assert
    CHECK(gameplay.isGameOver() == true);
    CHECK(gameplay.didPlayerWin() == true);
}