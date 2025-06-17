#include "../../../include/states/modes/SinglePlayerMode.h"
#include "../../../include/managers/InputManager.h"
#include "../../../include/core/GameConfig.h"
#include <random>
#include <ctime>
#include <ncurses.h>

SinglePlayerMode::SinglePlayerMode(std::shared_ptr<GameConfig> config)
    : GameModeStrategy(config)
{
  initializePlayers();
}

void SinglePlayerMode::initializePlayers()
{
  static bool seeded = false;
  if (!seeded)
  {
    srand(time(nullptr));
    seeded = true;
  }

  int actualWidth, actualHeight;
  getmaxyx(stdscr, actualHeight, actualWidth);

  int width = actualWidth;
  int height = actualHeight;

  int randomSide = rand() % GameConfig::SPAWN_SIDES_COUNT;
  auto spawnPos = getRandomPositionOnSide(randomSide, width, height);
  Direction safeDir = getSafeDirection(randomSide);

  auto player = std::make_unique<Player>(spawnPos.first, spawnPos.second, 1, safeDir);
  player->initializeTrail();

  players.clear();
  players.push_back(std::move(player));

  gameStartTime = std::chrono::steady_clock::now();
  gameFinished = false;
}

void SinglePlayerMode::handleInput(InputManager &inputManager, PlayState currentState)
{
  inputManager.pollInput();

  while (inputManager.hasInput())
  {
    int key = inputManager.getNextInput();

    if (InputManager::isMovementKey(key) && currentState == PLAYING)
    {
      if (InputManager::isPlayer1Key(key))
      {
        players[0]->setDirection(inputManager.keyToDirection(key));
      }
    }
  }
}

void SinglePlayerMode::update()
{
  if (players.empty())
    return;

  Player *player = players[0].get();
  int nextX = player->getNextX();
  int nextY = player->getNextY();

  int width = 80, height = 24;

  if (nextX <= 0 || nextX >= width - 1 || nextY <= 0 || nextY >= height - 1)
  {
    gameFinished = true;
    winner = 0;
    return;
  }

  const auto &trail = player->getTrail();
  if (trail.size() > GameConfig::MIN_TRAIL_LENGTH_FOR_COLLISION)
  {
    for (size_t i = 0; i < trail.size() - 2; i++)
    {
      if (trail[i].x == nextX && trail[i].y == nextY)
      {
        gameFinished = true;
        winner = 0;
        return;
      }
    }
  }

  player->move();
}

void SinglePlayerMode::reset()
{
  initializePlayers();
}

std::pair<int, int> SinglePlayerMode::getRandomPositionOnSide(int side, int width, int height)
{
  int margin = GameConfig::SPAWN_MARGIN;
  int x, y;

  switch (side)
  {
  case 0:
    x = margin + rand() % (width - 2 * margin);
    y = margin;
    break;
  case 1:
    x = width - margin;
    y = margin + rand() % (height - 2 * margin);
    break;
  case 2:
    x = margin + rand() % (width - 2 * margin);
    y = height - margin;
    break;
  case 3:
    x = margin;
    y = margin + rand() % (height - 2 * margin);
    break;
  }

  return std::make_pair(x, y);
}

Direction SinglePlayerMode::getSafeDirection(int side)
{
  switch (side)
  {
  case 0:
    return DOWN;
  case 1:
    return LEFT;
  case 2:
    return UP;
  case 3:
    return RIGHT;
  default:
    return RIGHT;
  }
}