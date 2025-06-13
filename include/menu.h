#pragma once

#include <ncurses.h>
#include <vector>
#include <string>
#include "types.h"

class Menu
{
private:
  MenuState currentState;
  int selectedOption;
  int menuWidth, menuHeight;

  // Menu options for different states
  std::vector<std::string> mainMenuOptions;
  std::vector<std::string> gameModeOptions;
  std::vector<std::string> settingsOptions;

  // Current settings
  GameSpeed currentGameSpeed;
  GameDifficulty currentDifficulty;
  int currentColorScheme;

  // Color definitions
  static const int COLOR_MENU_BORDER = 7;
  static const int COLOR_MENU_TEXT = 8;
  static const int COLOR_MENU_SELECTED = 9;
  static const int COLOR_MENU_TITLE = 10;

public:
  Menu();
  ~Menu();

  void init();
  void render();
  bool handleInput();

  MenuState getCurrentState() const { return currentState; }
  void setState(MenuState newState);
  int getSelectedOption() const { return selectedOption; }

  // Settings getters
  GameSpeed getGameSpeed() const { return currentGameSpeed; }
  GameDifficulty getDifficulty() const { return currentDifficulty; }
  int getColorScheme() const { return currentColorScheme; }

  void showMainMenu();
  void showGameModeMenu();
  void showSettingsMenu();
  void showGameSpeedMenu();
  void showDifficultyMenu();
  void showColorSchemeMenu();

  bool shouldStartGame() const;
  bool shouldQuit() const;

private:
  void initColors();
  void drawMenuBox(int startY, int startX, int height, int width, const std::string &title);
  void drawMenuOptions(const std::vector<std::string> &options, int startY, int startX);
  void resetSelection();
};
