#pragma once

#include "player.h"
#include <ncurses.h>
#include <vector>


enum GameState {
    PLAYING,
    GAME_OVER,
    PAUSED
};


class Game {
  private:
    int width, height;
    bool running;
    GameState state; // Current game state
  public:
    Game(int w, int h);
    ~Game();

    void init();
    void run();
    void update(Player &player);
    void render(Player &player);
    void handleInput(Player &player);
    void cleanup();

    bool checkWallCollision(int x, int y);
    bool checkTrailCollision(int x, int y, const Player &player);
    void gameOver();
    void restart();

    bool isRunning() const { return running; }
    void stop() { running = false; }
    GameState getState() const { return state; }
};