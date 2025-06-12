#pragma once

#include "player.h"
#include <ncurses.h>
#include <chrono>
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
    bool firstStart;

    std::chrono::steady_clock::time_point gameStartTime;
    std::chrono::steady_clock::time_point currentTime;
    int score;

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
    void startGame();

    void showWelcomeMessage();
    void renderHUD();

    void drawBorders();

    void updateScore();
    void getScore() const;
    int getGameTime() const;

    bool isRunning() const { return running; }
    void stop() { running = false; }
    GameState getState() const { return state; }
};