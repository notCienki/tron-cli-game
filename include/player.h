#pragma once

#include <vector>
#include <utility>

enum Direction {
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3
};

class Player {
  private:
    int x, y; // Player's position
    Direction direction; // Current direction of movement
    std::vector<std::pair<int, int>> trial;
    char symbol; // Character representing the player
  public:
    Player(int startX, int startY, char sym);

    void move();
    void setDirection(Direction newDir);
    void draw();

    int getNextX() const;
    int getNextY() const;

    int getX() const { return x; }
    int getY() const { return y; }
    Direction getDirection() const { return direction; }
    const std::vector<std::pair<int, int>>& getTrial() const { return trial; }
};