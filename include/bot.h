#pragma once

#include "player.h"
#include "types.h"
#include <vector>

class Bot
{
private:
  Player *botPlayer;
  int difficulty;

  Direction calculateBestMove(const Player &opponent, int width, int height);
  int evaluateMove(Direction dir, const Player &opponent, int width, int height);
  bool isPositionSafe(int x, int y, const Player &opponent, int width, int height);

public:
  Bot(int startX, int startY, int difficultyLevel = 1);
  ~Bot();

  void update(const Player &opponent, int width, int height);
  Player *getPlayer() const;
  void reset(int newX, int newY);
  void setDifficulty(int newDifficulty);
};