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
  GameMode currentGameMode;
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
  static const int COLOR_PLAYER2_HEAD = 9;
  static const int COLOR_PLAYER2_TRAIL = 10;

  void initColors();
  std::pair<int, int> getRandomSpawnPosition(int width, int height);
  std::pair<std::pair<int, int>, std::pair<int, int>> getTwoPlayerSpawnPositions(int width, int height);
  std::pair<int, int> getRandomPositionOnSide(int side, int width, int height);

  Direction getSafeDirection(int side);

public:
  Game(int w, int h);
  ~Game();

  void init();
  void run();
  void update(Player &player);
  void render(Player &player);
  void handleInput(Player &player);
  void handleInputTwoPlayer(Player &player1, Player &player2);
  void updateTwoPlayer(Player &player1, Player &player2);
  void renderTwoPlayer(Player &player1, Player &player2);
  void cleanup();

  bool checkWallCollision(int x, int y);
  bool checkTrailCollision(int x, int y, const Player &player);

  void gameOver();
  void restart(Player &player);
  void startGame();
  void gameOverTwoPlayer(int winner); // 1=P1 wins, 2=P2 wins, 0=tie
  void restartTwoPlayer(Player &player1, Player &player2);
  int winner; // Add to private section

  void showWelcomeMessage();
  void renderHUD();

  void drawBorders();

  void updateScore();
  int getScore() const;
  int getGameTime() const;
  void setGameSpeed(GameSpeed speed);
  void setColorScheme(int scheme);
  void setGameMode(GameMode mode);

  bool isRunning() const { return running; }
  void stop() { running = false; }
  GameState getState() const { return state; }
};