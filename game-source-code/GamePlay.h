#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <raylib-cpp.hpp>
#include <vector>
#include <memory>
#include "Level.h"
#include "Player.h"
#include "Monster.h"
#include "RedMonster.h"
#include "CollisionManager.h"
#include "InputHandler.h"
#include "Harpoon.h"

class GamePlay
{
public:
    GamePlay();
    ~GamePlay();
    void init();
    void handleInput();
    void update();
    void draw();
    bool isGameOver() const;
    bool isLevelComplete() const;
    bool didPlayerWin() const;
    void reset();
    Player &getPlayer();
    Level &getCurrentLevel();
    const std::vector<std::unique_ptr<Monster>> &getMonsters() const;

private:
    Level currentLevel;
    Player player;
    std::vector<std::unique_ptr<Monster>> monsters;
    bool gameOver;
    bool levelComplete;
    bool playerWon;

    void updateGameLogic();
    void drawHUD();
    void handlePlayerMovement();
    void initializeMonsters();
    void updateMonsters();
    void drawMonsters();
    bool checkWinCondition() const;
};

#endif // GAMEPLAY_H
