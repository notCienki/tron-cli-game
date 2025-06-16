#pragma once

#include <ncurses.h>
#include <memory>
#include <vector>
#include <string>
#include <unistd.h>
#include "../core/types.h"
#include "../core/GameConfig.h"

class Player;

class Renderer
{
private:
  std::shared_ptr<GameConfig> config;
  int screenWidth, screenHeight;

  static const int COLOR_PLAYER_HEAD = 1;
  static const int COLOR_PLAYER_TRAIL = 2;
  static const int COLOR_BORDERS = 3;
  static const int COLOR_GAME_OVER = 4;
  static const int COLOR_HUD = 5;
  static const int COLOR_MESSAGES = 6;
  static const int COLOR_PLAYER2_HEAD = 9;
  static const int COLOR_PLAYER2_TRAIL = 10;

public:
  explicit Renderer(std::shared_ptr<GameConfig> config);

  void initialize();
  void clear();
  void refresh();
  void cleanup();

  void drawBorders();
  void drawWelcomeMessage();
  void drawHUD(const std::string &mode, int score, int time);
  void drawGameOverScreen(int winner, int score, int time, const std::string &mode);

  void drawPlayers(const std::vector<Player *> &players);

  void initializeColors();
  void setColorScheme(int scheme);

  void updateScreenSize();
  int getScreenWidth() const { return screenWidth; }
  int getScreenHeight() const { return screenHeight; }

  void showWelcome();

  void renderGameScreen(GameState state, const std::vector<Player *> &players,
                        const std::string &mode, int score, int time, int winner = 0);
};