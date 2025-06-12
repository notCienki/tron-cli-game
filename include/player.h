#pragma once

#include <vector>
#include <utility>

enum Direction {
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3
};

struct TrailSegment{
    int x, y;
    Direction from, to;
    bool isHead;
    
    TrailSegment(int x, int y, Direction fromDir, Direction to, bool head = false)
        : x(x), y(y), from(fromDir), to(to), isHead(head) {}

    char getChar() const;
    const char* getUnicodeChar() const;
};

class Player {
  private:
    int x, y; // Player's position
    int startX, startY; // Starting position
    Direction direction; // Current direction of movement
    Direction lastDirection;
    std::vector<TrailSegment> trail;


  public:
    Player(int startX, int startY);

    void move();
    void setDirection(Direction newDir);
    void draw();
    void reset();

    char getPlayerChar() const;
    const char* getPlayerUnicodeChar() const;

    int getNextX() const;
    int getNextY() const;
    int getX() const { return x; }
    int getY() const { return y; }
    Direction getDirection() const { return direction; }
    const std::vector<TrailSegment>& getTrail() const { return trail; }
};