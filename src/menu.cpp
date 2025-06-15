#include "../include/menu.h"

Menu::Menu() : currentState(MAIN_MENU), selectedOption(0), currentGameSpeed(NORMAL), currentDifficulty(MEDIUM), currentColorScheme(0), currentGameMode(SINGLE_PLAYER)
{
  mainMenuOptions = {
      "Start Game",
      "Game Mode",
      "Settings",
      "Quit"};
  gameModeOptions = {
      "Single Player",
      "Two Player",
      "vs Bot",
      "Back"};
  settingsOptions = {
      "Game Speed",
      "Difficulty",
      "Colors",
      "Back"};
}

Menu::~Menu()
{
  endwin();
}

void Menu::init()
{
  initColors();
}

void Menu::initColors()
{
  if (has_colors())
  {
    start_color();
    use_default_colors();

    init_pair(COLOR_MENU_BORDER, COLOR_CYAN, -1);
    init_pair(COLOR_MENU_TEXT, COLOR_WHITE, -1);
    init_pair(COLOR_MENU_SELECTED, COLOR_YELLOW, -1);
    init_pair(COLOR_MENU_TITLE, COLOR_GREEN, -1);
  }
}

void Menu::render()
{
  clear();

  int termHeight, termWidth;
  getmaxyx(stdscr, termHeight, termWidth);
  (void)termHeight;

  switch (currentState)
  {
  case MAIN_MENU:
    showMainMenu();
    break;
  case GAME_MODE_MENU:
    showGameModeMenu();
    break;
  case SETTINGS_MENU:
    showSettingsMenu();
    break;
  case GAME_SPEED_MENU:
    showGameSpeedMenu();
    break;
  case DIFFICULTY_MENU:
    showDifficultyMenu();
    break;
  case COLOR_SCHEME_MENU:
    showColorSchemeMenu();
    break;
  case IN_GAME:
    break;
  }

  refresh();
}

void Menu::showMainMenu()
{
  int termHeight, termWidth;
  getmaxyx(stdscr, termHeight, termWidth);

  int centerX = termWidth / 2;
  int centerY = termHeight / 2;

  attron(COLOR_PAIR(COLOR_MENU_TITLE));
  mvprintw(centerY - 4, centerX - 12, "╔══════════════════════╗");
  mvprintw(centerY - 3, centerX - 12, "║      TRON GAME       ║");
  mvprintw(centerY - 2, centerX - 12, "╠══════════════════════╣");
  attroff(COLOR_PAIR(COLOR_MENU_TITLE));

  attron(COLOR_PAIR(COLOR_MENU_TEXT));

  if (selectedOption == 0)
  {
    attron(COLOR_PAIR(COLOR_MENU_SELECTED));
    mvprintw(centerY - 1, centerX - 12, "║ > Start Game         ║");
    attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
  }
  else
  {
    mvprintw(centerY - 1, centerX - 12, "║   Start Game         ║");
  }

  if (selectedOption == 1)
  {
    attron(COLOR_PAIR(COLOR_MENU_SELECTED));
    mvprintw(centerY, centerX - 12, "║ > Game Mode          ║");
    attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
  }
  else
  {
    mvprintw(centerY, centerX - 12, "║   Game Mode          ║");
  }

  if (selectedOption == 2)
  {
    attron(COLOR_PAIR(COLOR_MENU_SELECTED));
    mvprintw(centerY + 1, centerX - 12, "║ > Settings           ║");
    attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
  }
  else
  {
    mvprintw(centerY + 1, centerX - 12, "║   Settings           ║");
  }

  if (selectedOption == 3)
  {
    attron(COLOR_PAIR(COLOR_MENU_SELECTED));
    mvprintw(centerY + 2, centerX - 12, "║ > Quit               ║");
    attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
  }
  else
  {
    mvprintw(centerY + 2, centerX - 12, "║   Quit               ║");
  }

  mvprintw(centerY + 3, centerX - 12, "╚══════════════════════╝");
  attroff(COLOR_PAIR(COLOR_MENU_TEXT));
}

bool Menu::handleInput()
{
  int ch = getch();
  switch (ch)
  {
  case KEY_UP:
    if (selectedOption > 0)
    {
      selectedOption--;
    }
    break;
  case KEY_DOWN:
  {
    int maxOptions = 0;
    if (currentState == MAIN_MENU)
      maxOptions = mainMenuOptions.size();
    else if (currentState == GAME_MODE_MENU)
      maxOptions = gameModeOptions.size();
    else if (currentState == SETTINGS_MENU)
      maxOptions = settingsOptions.size();
    else if (currentState == GAME_SPEED_MENU)
      maxOptions = 4;
    else if (currentState == COLOR_SCHEME_MENU)
      maxOptions = 4;

    if (selectedOption < maxOptions - 1)
    {
      selectedOption++;
    }
    break;
  }
  case '\n':
  case ' ':
    if (currentState == MAIN_MENU)
    {
      if (selectedOption == 1)
      {
        currentState = GAME_MODE_MENU;
        resetSelection();
      }
      else if (selectedOption == 2)
      {
        currentState = SETTINGS_MENU;
        resetSelection();
      }
    }
    else if (currentState == GAME_MODE_MENU)
    {
      if (selectedOption == 0)
      {
        currentGameMode = SINGLE_PLAYER;
        currentState = MAIN_MENU;
        selectedOption = 0;
        resetSelection();
      }
      else if (selectedOption == 1)
      {
        currentGameMode = TWO_PLAYER;
        currentState = MAIN_MENU;
        selectedOption = 0;
        resetSelection();
      }
      else if (selectedOption == 2)
      {
        currentGameMode = VS_BOT;
        currentState = MAIN_MENU;
        selectedOption = 0;
        resetSelection();
      }
      else if (selectedOption == 3)
      {
        currentState = MAIN_MENU;
        selectedOption = 1;
      }
    }
    else if (currentState == SETTINGS_MENU)
    {
      if (selectedOption == 0)
      {
        currentState = GAME_SPEED_MENU;
        resetSelection();
      }
      else if (selectedOption == 1)
      {
        currentState = DIFFICULTY_MENU;
        resetSelection();
      }
      else if (selectedOption == 2)
      {
        currentState = COLOR_SCHEME_MENU;
        resetSelection();
      }
      else if (selectedOption == 3)
      {
        currentState = MAIN_MENU;
        selectedOption = 2;
      }
    }
    else if (currentState == GAME_SPEED_MENU)
    {
      if (selectedOption == 0)
      {
        currentGameSpeed = SLOW;
        currentState = SETTINGS_MENU;
        selectedOption = 0;
      }
      else if (selectedOption == 1)
      {
        currentGameSpeed = NORMAL;
        currentState = SETTINGS_MENU;
        selectedOption = 0;
      }
      else if (selectedOption == 2)
      {
        currentGameSpeed = FAST;
        currentState = SETTINGS_MENU;
        selectedOption = 0;
      }
      else if (selectedOption == 3)
      {
        currentState = SETTINGS_MENU;
        selectedOption = 0;
      }
    }
    else if (currentState == DIFFICULTY_MENU)
    {
      if (selectedOption == 0)
      {
        currentDifficulty = EASY;
        currentState = SETTINGS_MENU;
        selectedOption = 1;
      }
      else if (selectedOption == 1)
      {
        currentDifficulty = MEDIUM;
        currentState = SETTINGS_MENU;
        selectedOption = 1;
      }
      else if (selectedOption == 2)
      {
        currentDifficulty = HARD;
        currentState = SETTINGS_MENU;
        selectedOption = 1;
      }
      else if (selectedOption == 3)
      {
        currentState = SETTINGS_MENU;
        selectedOption = 1;
      }
    }
    else if (currentState == COLOR_SCHEME_MENU)
    {
      if (selectedOption >= 0 && selectedOption < 3)
      {
        currentColorScheme = selectedOption;
        currentState = SETTINGS_MENU;
        selectedOption = 2;
      }
      else if (selectedOption == 3)
      {
        currentState = SETTINGS_MENU;
        selectedOption = 2;
      }
    }

    return true;
  case 'q':
  case 'Q':
  case 27:
    currentState = MAIN_MENU;
    selectedOption = 3;
    return true;
  }
  return false;
}

bool Menu::shouldStartGame() const
{
  return (currentState == MAIN_MENU && selectedOption == 0);
}

bool Menu::shouldQuit() const
{
  return (currentState == MAIN_MENU && selectedOption == 3);
}

void Menu::setState(MenuState newState)
{
  currentState = newState;
  resetSelection();
}

void Menu::resetSelection()
{
  selectedOption = 0;
}

void Menu::showGameModeMenu()
{
  int termHeight, termWidth;
  getmaxyx(stdscr, termHeight, termWidth);

  int centerX = termWidth / 2;
  int centerY = termHeight / 2;

  attron(COLOR_PAIR(COLOR_MENU_TITLE));
  mvprintw(centerY - 4, centerX - 12, "╔══════════════════════╗");
  mvprintw(centerY - 3, centerX - 12, "║    GAME MODE MENU    ║");
  mvprintw(centerY - 2, centerX - 12, "╠══════════════════════╣");
  attroff(COLOR_PAIR(COLOR_MENU_TITLE));

  attron(COLOR_PAIR(COLOR_MENU_TEXT));

  if (selectedOption == 0)
  {
    attron(COLOR_PAIR(COLOR_MENU_SELECTED));
    mvprintw(centerY - 1, centerX - 12, "║ > Single Player      ║");
    attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
  }
  else
  {
    mvprintw(centerY - 1, centerX - 12, "║   Single Player      ║");
  }

  if (selectedOption == 1)
  {
    attron(COLOR_PAIR(COLOR_MENU_SELECTED));
    mvprintw(centerY, centerX - 12, "║ > Two Player         ║");
    attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
  }
  else
  {
    mvprintw(centerY, centerX - 12, "║   Two Player         ║");
  }

  if (selectedOption == 2)
  {
    attron(COLOR_PAIR(COLOR_MENU_SELECTED));
    mvprintw(centerY + 1, centerX - 12, "║ > vs Bot             ║");
    attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
  }
  else
  {
    mvprintw(centerY + 1, centerX - 12, "║   vs Bot             ║");
  }

  if (selectedOption == 3)
  {
    attron(COLOR_PAIR(COLOR_MENU_SELECTED));
    mvprintw(centerY + 2, centerX - 12, "║ > Back               ║");
    attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
  }
  else
  {
    mvprintw(centerY + 2, centerX - 12, "║   Back               ║");
  }

  mvprintw(centerY + 3, centerX - 12, "╚══════════════════════╝");
  attroff(COLOR_PAIR(COLOR_MENU_TEXT));
}

void Menu::showSettingsMenu()
{
  int termHeight, termWidth;
  getmaxyx(stdscr, termHeight, termWidth);

  int centerX = termWidth / 2;
  int centerY = termHeight / 2;

  attron(COLOR_PAIR(COLOR_MENU_TITLE));
  mvprintw(centerY - 4, centerX - 12, "╔══════════════════════╗");
  mvprintw(centerY - 3, centerX - 12, "║     SETTINGS MENU    ║");
  mvprintw(centerY - 2, centerX - 12, "╠══════════════════════╣");
  attroff(COLOR_PAIR(COLOR_MENU_TITLE));

  attron(COLOR_PAIR(COLOR_MENU_TEXT));

  if (selectedOption == 0)
  {
    attron(COLOR_PAIR(COLOR_MENU_SELECTED));
    mvprintw(centerY - 1, centerX - 12, "║ > Game Speed         ║");
    attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
  }
  else
  {
    mvprintw(centerY - 1, centerX - 12, "║   Game Speed         ║");
  }

  if (selectedOption == 1)
  {
    attron(COLOR_PAIR(COLOR_MENU_SELECTED));
    mvprintw(centerY, centerX - 12, "║ > Difficulty         ║");
    attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
  }
  else
  {
    mvprintw(centerY, centerX - 12, "║   Difficulty         ║");
  }

  if (selectedOption == 2)
  {
    attron(COLOR_PAIR(COLOR_MENU_SELECTED));
    mvprintw(centerY + 1, centerX - 12, "║ > Colors             ║");
    attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
  }
  else
  {
    mvprintw(centerY + 1, centerX - 12, "║   Colors             ║");
  }

  if (selectedOption == 3)
  {
    attron(COLOR_PAIR(COLOR_MENU_SELECTED));
    mvprintw(centerY + 2, centerX - 12, "║ > Back               ║");
    attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
  }
  else
  {
    mvprintw(centerY + 2, centerX - 12, "║   Back               ║");
  }

  mvprintw(centerY + 3, centerX - 12, "╚══════════════════════╝");
  attroff(COLOR_PAIR(COLOR_MENU_TEXT));
}

void Menu::showGameSpeedMenu()
{
  int termHeight, termWidth;
  getmaxyx(stdscr, termHeight, termWidth);

  int centerX = termWidth / 2;
  int centerY = termHeight / 2;

  attron(COLOR_PAIR(COLOR_MENU_TITLE));
  mvprintw(centerY - 4, centerX - 12, "╔══════════════════════╗");
  mvprintw(centerY - 3, centerX - 12, "║     GAME SPEED       ║");
  mvprintw(centerY - 2, centerX - 12, "╠══════════════════════╣");
  attroff(COLOR_PAIR(COLOR_MENU_TITLE));

  attron(COLOR_PAIR(COLOR_MENU_TEXT));

  if (selectedOption == 0)
  {
    attron(COLOR_PAIR(COLOR_MENU_SELECTED));
    mvprintw(centerY - 1, centerX - 12, "║ > Slow               ║");
    attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
  }
  else
  {
    mvprintw(centerY - 1, centerX - 12, "║   Slow               ║");
  }

  if (selectedOption == 1)
  {
    attron(COLOR_PAIR(COLOR_MENU_SELECTED));
    mvprintw(centerY, centerX - 12, "║ > Normal             ║");
    attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
  }
  else
  {
    mvprintw(centerY, centerX - 12, "║   Normal             ║");
  }

  if (selectedOption == 2)
  {
    attron(COLOR_PAIR(COLOR_MENU_SELECTED));
    mvprintw(centerY + 1, centerX - 12, "║ > Fast               ║");
    attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
  }
  else
  {
    mvprintw(centerY + 1, centerX - 12, "║   Fast               ║");
  }

  if (selectedOption == 3)
  {
    attron(COLOR_PAIR(COLOR_MENU_SELECTED));
    mvprintw(centerY + 2, centerX - 12, "║ > Back               ║");
    attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
  }
  else
  {
    mvprintw(centerY + 2, centerX - 12, "║   Back               ║");
  }
  mvprintw(centerY + 3, centerX - 12, "╚══════════════════════╝");
  attroff(COLOR_PAIR(COLOR_MENU_TEXT));
}

void Menu::showDifficultyMenu()
{
  int termHeight, termWidth;
  getmaxyx(stdscr, termHeight, termWidth);

  int centerX = termWidth / 2;
  int centerY = termHeight / 2;

  attron(COLOR_PAIR(COLOR_MENU_TITLE));
  mvprintw(centerY - 4, centerX - 12, "╔══════════════════════╗");
  mvprintw(centerY - 3, centerX - 12, "║     DIFFICULTY       ║");
  mvprintw(centerY - 2, centerX - 12, "╠══════════════════════╣");
  attroff(COLOR_PAIR(COLOR_MENU_TITLE));

  attron(COLOR_PAIR(COLOR_MENU_TEXT));

  if (selectedOption == 0)
  {
    attron(COLOR_PAIR(COLOR_MENU_SELECTED));
    mvprintw(centerY - 1, centerX - 12, "║ > Easy               ║");
    attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
  }
  else
  {
    mvprintw(centerY - 1, centerX - 12, "║   Easy               ║");
  }

  if (selectedOption == 1)
  {
    attron(COLOR_PAIR(COLOR_MENU_SELECTED));
    mvprintw(centerY, centerX - 12, "║ > Medium             ║");
    attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
  }
  else
  {
    mvprintw(centerY, centerX - 12, "║   Medium             ║");
  }

  if (selectedOption == 2)
  {
    attron(COLOR_PAIR(COLOR_MENU_SELECTED));
    mvprintw(centerY + 1, centerX - 12, "║ > Hard               ║");
    attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
  }
  else
  {
    mvprintw(centerY + 1, centerX - 12, "║   Hard               ║");
  }

  if (selectedOption == 3)
  {
    attron(COLOR_PAIR(COLOR_MENU_SELECTED));
    mvprintw(centerY + 2, centerX - 12, "║ > Back               ║");
    attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
  }
  else
  {
    mvprintw(centerY + 2, centerX - 12, "║   Back               ║");
  }
  mvprintw(centerY + 3, centerX - 12, "╚══════════════════════╝");
  attroff(COLOR_PAIR(COLOR_MENU_TEXT));
}

void Menu::showColorSchemeMenu()
{
  int termHeight, termWidth;
  getmaxyx(stdscr, termHeight, termWidth);

  int centerX = termWidth / 2;
  int centerY = termHeight / 2;

  attron(COLOR_PAIR(COLOR_MENU_TITLE));
  mvprintw(centerY - 4, centerX - 12, "╔══════════════════════╗");
  mvprintw(centerY - 3, centerX - 12, "║     COLOR SCHEME     ║");
  mvprintw(centerY - 2, centerX - 12, "╠══════════════════════╣");
  attroff(COLOR_PAIR(COLOR_MENU_TITLE));

  attron(COLOR_PAIR(COLOR_MENU_TEXT));

  for (int i = 0; i < 3; ++i)
  {
    if (selectedOption == i)
    {
      attron(COLOR_PAIR(COLOR_MENU_SELECTED));
      mvprintw(centerY - 1 + i, centerX - 12, "║ > Color Scheme %d     ║", i + 1);
      attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
    }
    else
    {
      mvprintw(centerY - 1 + i, centerX - 12, "║   Color Scheme %d     ║", i + 1);
    }
  }

  if (selectedOption == 3)
  {
    attron(COLOR_PAIR(COLOR_MENU_SELECTED));
    mvprintw(centerY + 2, centerX - 12, "║ > Back               ║");
    attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
  }
  else
  {
    mvprintw(centerY + 2, centerX - 12, "║   Back               ║");
  }

  mvprintw(centerY + 3, centerX - 12, "╚══════════════════════╝");
  attroff(COLOR_PAIR(COLOR_MENU_TEXT));
}