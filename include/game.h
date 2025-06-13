#pragma once

#include "player.h"
#include <ncurses.h>
#include <chrono>
#include <locale.h>
#include <vector>
#include "types.h"

class Game
{
private:
  int width, height;
  bool running;
  GameState state; // Current game state
  GameSpeed currentGameSpeed;
  int currentColorScheme;
  bool firstStart;

  std::chrono::steady_clock::time_point gameStartTime;
  std::chrono::steady_clock::time_point currentTime;
  int score;

  static const int COLOR_PLAYER_HEAD = 1;
  static const int COLOR_PLAYER_TRAIL = 2;
  static const int COLOR_BORDERS = 3;
  static const int COLOR_GAME_OVER = 4;
  static const int COLOR_HUD = 5;
  static const int COLOR_MESSAGES = 6;

  void initColors();
  std::pair<int, int> getRandomSpawnPosition(int widht, int height);

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
  void restart(Player &player);
  void startGame();

  void showWelcomeMessage();
  void renderHUD();

  void drawBorders();

  void updateScore();
  int getScore() const;
  int getGameTime() const;
  void setGameSpeed(GameSpeed speed);
  void setColorScheme(int scheme);

  bool isRunning() const { return running; }
  void stop() { running = false; }
  GameState getState() const { return state; }
};