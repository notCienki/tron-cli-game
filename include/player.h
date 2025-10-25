#pragma once

#include <vector>
#include <utility>
#include "types.h"
#include "config.h"

struct TrailSegment
{
  int x, y;
  Direction from, to;
  bool isHead;

  TrailSegment(int x, int y, Direction fromDir, Direction to, bool head = false)
      : x(x), y(y), from(fromDir), to(to), isHead(head) {}

  char getChar() const;
  const char *getUnicodeChar() const;
};

class Player
{
private:
  int x, y;
  int startX, startY;
  Direction direction;
  Direction lastDirection;
  std::vector<TrailSegment> trail;
  int playerId;

public:
  Player(int startX, int startY, int id = Config::PLAYER_1_ID, Direction startDirection = RIGHT);

  void move();
  void setDirection(Direction newDir);
  void draw();
  void reset(int newX = -1, int newY = -1);

  void initializeTrail();

  char getPlayerChar() const;
  const char *getPlayerUnicodeChar() const;

  int getNextX() const;
  int getNextY() const;
  int getX() const { return x; }
  int getY() const { return y; }
  Direction getDirection() const { return direction; }
  const std::vector<TrailSegment> &getTrail() const { return trail; }
};