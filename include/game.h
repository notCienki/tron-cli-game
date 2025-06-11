#pragma once

#include "player.h"
#include <ncurses.h>
#include <vector>

class Game {
  private:
    int width, height;
    bool running;
  public:
    Game(int w, int h);
    ~Game();

    void init();
    void run();
    void update(Player &player);
    void render(Player &player);
    void handleInput(Player &player);
    void cleanup();

    bool isRunning() const { return running; }
    void stop() { running = false; }
};