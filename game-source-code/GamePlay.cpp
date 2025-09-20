#include "GamePlay.h"

GamePlay::GamePlay()
    : gameOver(false), levelComplete(false), playerWon(false)
{
}

GamePlay::~GamePlay()
{
}

void GamePlay::init()
{
    currentLevel.initializeDefault();
    player.reset(currentLevel.getPlayerStartPosition());
    initializeMonsters();
    gameOver = false;
    levelComplete = false;
    playerWon = false;
}

void GamePlay::handleInput()
{
    if (!gameOver && !levelComplete)
    {
        handlePlayerMovement();
    }

    if (InputHandler::isMenuPressed())
    {
        // This will be handled by GameStateManager
    }
}

void GamePlay::update()
{
    if (!gameOver && !levelComplete)
    {
        player.update();
        updateMonsters();
        updateGameLogic();
    }
}

void GamePlay::draw()
{
    ClearBackground(BLACK);
    currentLevel.draw();
    player.draw();
    drawMonsters();
    drawHUD();

    if (gameOver)
    {
        const char *message = playerWon ? "LEVEL COMPLETE!" : "GAME OVER";
        Color color = playerWon ? GREEN : RED;
        int textWidth = MeasureText(message, 40);
        DrawText(message, (GetScreenWidth() - textWidth) / 2,
                 GetScreenHeight() / 2 - 20, 40, color);

        const char *instruction = "Press ESC to return to menu";
        int instrWidth = MeasureText(instruction, 20);
        DrawText(instruction, (GetScreenWidth() - instrWidth) / 2,
                 GetScreenHeight() / 2 + 30, 20, WHITE);
    }
}

bool GamePlay::isGameOver() const
{
    return gameOver;
}

bool GamePlay::isLevelComplete() const
{
    return levelComplete;
}

bool GamePlay::didPlayerWin() const
{
    return playerWon;
}

void GamePlay::reset()
{
    init();
}

Player &GamePlay::getPlayer()
{
    return player;
}

Level &GamePlay::getCurrentLevel()
{
    return currentLevel;
}

const std::vector<std::unique_ptr<Monster>> &GamePlay::getMonsters() const
{
    return monsters;
}

void GamePlay::updateGameLogic()
{
    std::vector<Monster *> monsterPtrs;
    for (const auto &monster : monsters)
    {
        monsterPtrs.push_back(monster.get());
    }

    if (CollisionManager::checkPlayerMonsterCollision(player, monsterPtrs))
    {
        gameOver = true;
        playerWon = false;
        return;
    }

    Harpoon &harpoon = player.getHarpoon();
    if (harpoon.isActive())
    {
        if (harpoon.getState() == HarpoonState::FLYING)
        {
            Monster *hitMonster = CollisionManager::checkHarpoonMonsterCollision(harpoon, monsterPtrs, currentLevel.getGrid());
            if (hitMonster)
            {
                hitMonster->setState(MonsterState::DEAD);
                harpoon.setState(HarpoonState::INFLATING);
                harpoon.setPosition(hitMonster->getPosition());
            }

            if (CollisionManager::checkHarpoonBounds(harpoon, currentLevel.getGrid()))
            {
                harpoon.reset();
            }
        }
    }

    if (checkWinCondition())
    {
        levelComplete = true;
        playerWon = true;
        gameOver = true;
    }
}

void GamePlay::drawHUD()
{
    const char *title = "DIG DUG";
    DrawText(title, 10, 10, 20, ORANGE);

    int aliveMonsters = 0;
    for (const auto &monster : monsters)
    {
        if (monster->isActive() && monster->getState() != MonsterState::DEAD)
            aliveMonsters++;
    }
    const char *monsterText = TextFormat("Monsters: %d", aliveMonsters);
    DrawText(monsterText, 10, 35, 15, WHITE);
}

void GamePlay::handlePlayerMovement()
{
    Direction moveDirection = InputHandler::getDirectionInput();

    if (moveDirection != Direction::NONE)
    {
        player.move(moveDirection, currentLevel.getGrid());
    }

    if (InputHandler::isActionPressed())
    {
        player.shootHarpoon();
    }
}

void GamePlay::initializeMonsters()
{
    monsters.clear();

    std::vector<Vector2> spawnPositions = currentLevel.getMonsterSpawnPositions();

    for (const Vector2 &spawnPos : spawnPositions)
    {
        auto monster = std::make_unique<RedMonster>(spawnPos);
        monsters.push_back(std::move(monster));
    }
}

void GamePlay::updateMonsters()
{
    Vector2 playerPos = player.getPosition();

    for (auto &monster : monsters)
    {
        if (monster->isActive() && monster->getState() != MonsterState::DEAD)
        {
            RedMonster *redMonster = dynamic_cast<RedMonster *>(monster.get());
            if (redMonster)
            {
                redMonster->updateAI(playerPos, currentLevel.getGrid());
            }

            monster->update();
        }
    }
}

void GamePlay::drawMonsters()
{
    for (const auto &monster : monsters)
    {
        if (monster->isActive() && monster->getState() != MonsterState::DEAD)
        {
            monster->draw();
        }
    }
}

bool GamePlay::checkWinCondition() const
{
    for (const auto &monster : monsters)
    {
        if (monster->isActive() && monster->getState() != MonsterState::DEAD)
        {
            return false;
        }
    }
    return true;
}