#include "../include/bot.h"
#include <random>
#include <algorithm>
#include <queue>
#include <set>

Bot::Bot(int startX, int startY, Direction startDirection)
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
  Direction currentDir = botPlayer->getDirection();

  int currentX = botPlayer->getX();
  int currentY = botPlayer->getY();

  std::vector<std::pair<Direction, int>> spaceAnalysis;
  int maxSpace = -1;
  Direction bestSpaceDir = currentDir;

  for (int i = 0; i < 4; i++)
  {
    Direction dir = directions[i];
    int nextX = currentX, nextY = currentY;

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
      int space = floodFill(nextX, nextY, opponent, width, height);
      spaceAnalysis.push_back({dir, space});
      if (space > maxSpace)
      {
        maxSpace = space;
        bestSpaceDir = dir;
      }
    }
  }

  if (spaceAnalysis.empty())
  {
    return currentDir;
  }

  int bestScore = -10000;
  Direction bestDir = bestSpaceDir;

  for (const auto &analysis : spaceAnalysis)
  {
    Direction dir = analysis.first;
    int availableSpace = analysis.second;
    int score = 0;

    int nextX = currentX, nextY = currentY;
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

    score += availableSpace * 50;

    int distFromLeft = nextX;
    int distFromRight = width - 1 - nextX;
    int distFromTop = nextY;
    int distFromBottom = height - 1 - nextY;
    int minDistToWall = std::min({distFromLeft, distFromRight, distFromTop, distFromBottom});
    score += minDistToWall * 15;

    if (dir == currentDir)
    {
      if (availableSpace >= maxSpace * 0.9)
      {
        score += 100;
      }
      else if (availableSpace >= maxSpace * 0.75)
      {
        score += 30;
      }
    }

    int lookAheadSteps = 0;
    int checkX = nextX, checkY = nextY;
    for (int step = 0; step < 10; step++)
    {
      switch (dir)
      {
      case UP:
        checkY--;
        break;
      case DOWN:
        checkY++;
        break;
      case LEFT:
        checkX--;
        break;
      case RIGHT:
        checkX++;
        break;
      }

      if (isPositionSafe(checkX, checkY, opponent, width, height))
      {
        lookAheadSteps++;
      }
      else
      {
        break;
      }
    }
    score += lookAheadSteps * 8;

    int futureOptions = 0;
    Direction futureDir[] = {UP, DOWN, LEFT, RIGHT};
    for (int j = 0; j < 4; j++)
    {
      int futureX = nextX, futureY = nextY;
      switch (futureDir[j])
      {
      case UP:
        futureY--;
        break;
      case DOWN:
        futureY++;
        break;
      case LEFT:
        futureX--;
        break;
      case RIGHT:
        futureX++;
        break;
      }
      if (isPositionSafe(futureX, futureY, opponent, width, height))
      {
        futureOptions++;
      }
    }
    score += futureOptions * 20;

    int distToOpponent = abs(nextX - opponent.getX()) + abs(nextY - opponent.getY());
    if (distToOpponent > 5)
    {
      score += 10;
    }

    if (score > bestScore)
    {
      bestScore = score;
      bestDir = dir;
    }
  }

  return bestDir;
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

int Bot::floodFill(int startX, int startY, const Player &opponent, int width, int height)
{
  std::queue<std::pair<int, int>> q;
  std::set<std::pair<int, int>> visited;

  q.push({startX, startY});
  visited.insert({startX, startY});

  int accessibleCells = 0;
  const int MAX_DEPTH = 100;

  while (!q.empty() && accessibleCells < MAX_DEPTH)
  {
    auto [x, y] = q.front();
    q.pop();
    accessibleCells++;

    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};

    for (int i = 0; i < 4; i++)
    {
      int nx = x + dx[i];
      int ny = y + dy[i];

      if (visited.find({nx, ny}) == visited.end() &&
          isPositionSafe(nx, ny, opponent, width, height))
      {
        visited.insert({nx, ny});
        q.push({nx, ny});
      }
    }
  }

  return accessibleCells;
}
