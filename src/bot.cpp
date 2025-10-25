#include "../include/bot.h"
#include <random>
#include <algorithm>

Bot::Bot(int startX, int startY, int difficultyLevel, Direction startDirection) : difficulty(difficultyLevel)
{
  botPlayer = new Player(startX, startY, Config::PLAYER_2_ID, startDirection);
}

Bot::~Bot()
{
  delete botPlayer;
}

void Bot::update(const Player &opponent, int width, int height)
{
  Direction nextMove = calculateBestMove(opponent, width, height);
  botPlayer->setDirection(nextMove);
  botPlayer->move();
}

Direction Bot::calculateBestMove(const Player &opponent, int width, int height)
{
  Direction directions[] = {UP, DOWN, LEFT, RIGHT};

  int currentX = botPlayer->getX();
  int currentY = botPlayer->getY();

  for (int i = 0; i < Config::NUM_SIDES; i++)
  {
    Direction dir = directions[i];
    int nextX = currentX;
    int nextY = currentY;

    switch (dir)
    {
    case UP:
      nextY--;
      break;
    case DOWN:
      nextY++;
      break;
    case LEFT:
      nextX--;
      break;
    case RIGHT:
      nextX++;
      break;
    }

    if (isPositionSafe(nextX, nextY, opponent, width, height))
    {
      return dir;
    }
  }
  return RIGHT;
}

bool Bot::isPositionSafe(int x, int y, const Player &opponent, int width, int height)
{
  if (x <= 0 || x >= width - 1 || y <= 0 || y >= height - 1)
  {
    return false;
  }

  const auto &botTrail = botPlayer->getTrail();
  for (const auto &segment : botTrail)
  {
    if (segment.x == x && segment.y == y)
    {
      return false;
    }
  }

  const auto &opponentTrail = opponent.getTrail();
  for (const auto &segment : opponentTrail)
  {
    if (segment.x == x && segment.y == y)
    {
      return false;
    }
  }
  return true;
}

Player *Bot::getPlayer() const
{
  return botPlayer;
}

void Bot::reset(int newX, int newY)
{
  if (newX != -1 && newY != -1)
  {
    botPlayer->reset(newX, newY);
  }
}

void Bot::setDifficulty(int newDifficulty)
{
  difficulty = newDifficulty;
}
