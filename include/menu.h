#pragma once

#include <ncurses.h>
#include <vector>
#include <string>
#include "types.h"
#include "config.h"

class Menu
{
private:
  MenuState currentState;
  int selectedOption;
  int menuWidth, menuHeight;

  std::vector<std::string> mainMenuOptions;
  std::vector<std::string> gameModeOptions;
  std::vector<std::string> settingsOptions;

  GameSpeed currentGameSpeed;
  int currentColorScheme;
  GameMode currentGameMode;

public:
  Menu();
  ~Menu();

  void init();
  void render();
  bool handleInput();

  MenuState getCurrentState() const { return currentState; }
  void setState(MenuState newState);
  int getSelectedOption() const { return selectedOption; }

  GameSpeed getGameSpeed() const { return currentGameSpeed; }
  int getColorScheme() const { return currentColorScheme; }
  GameMode getGameMode() const { return currentGameMode; }

  void showMainMenu();
  void showGameModeMenu();
  void showSettingsMenu();
  void showGameSpeedMenu();
  void showColorSchemeMenu();

  bool shouldStartGame() const;
  bool shouldQuit() const;

private:
  void initColors();
  void drawMenuBox(int startY, int startX, int height, int width, const std::string &title);
  void drawMenuOptions(const std::vector<std::string> &options, int startY, int startX);
  void resetSelection();
};
