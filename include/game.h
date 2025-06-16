#pragma once

#include "entities/player.h"
#include "entities/bot.h"
#include <ncurses.h>
#include <chrono>
#include <locale.h>
#include <vector>
#include "core/types.h"
#include "managers/InputManager.h"
#include <memory>
#include "rendering/Renderer.h"

class Game
{
private:
  int width, height;
  bool running;
  GameState state;
  GameSpeed currentGameSpeed;
  GameMode currentGameMode;
  int currentColorScheme;
  bool firstStart;

  Bot *gameBot;

  std::chrono::steady_clock::time_point gameStartTime;
  std::chrono::steady_clock::time_point currentTime;
  int score;

  void initColors();
  std::pair<int, int> getRandomSpawnPosition(int width, int height);
  std::pair<std::pair<int, int>, std::pair<int, int>> getTwoPlayerSpawnPositions(int width, int height);
  std::pair<int, int> getRandomPositionOnSide(int side, int width, int height);

  Direction getSafeDirection(int side);

  std::unique_ptr<InputManager> inputManager;
  std::shared_ptr<Renderer> renderer;
  std::shared_ptr<GameConfig> gameConfig;

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
  void handleInputVsBot(Player &player, Bot &bot);
  void updateVsBot(Player &player, Bot &bot);
  void renderVsBot(Player &player, Bot &bot);
  void restartVsBot(Player &player, Bot &bot);
  void cleanup();

  bool checkWallCollision(int x, int y);
  bool checkTrailCollision(int x, int y, const Player &player);

  void gameOver();
  void restart(Player &player);
  void startGame();
  void gameOverTwoPlayer(int winner);
  void restartTwoPlayer(Player &player1, Player &player2);
  int winner;

  void updateScore();
  int getScore() const;
  int getGameTime() const;
  void setGameSpeed(GameSpeed speed);
  void setGameMode(GameMode mode);

  bool isRunning() const { return running; }
  void stop() { running = false; }
  GameState getState() const { return state; }
};