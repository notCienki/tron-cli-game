#include "../include/menu.h"

Menu::Menu() : currentState(MAIN_MENU), selectedOption(0)
{
  mainMenuOptions = {
      "Start Game",
      "Game Mode",
      "Settings",
      "Quit"};
  gameModeOptions = {
      "Single Player",
      "Two Player (future)",
      "vs Bot (future)",
      "Back"};
  settingsOptions = {
      "Game Speed",
      "Difficulty",
      "Colors",
      "Back"};
}

Menu::~Menu()
{
  endwin(); // Clean up ncurses
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
  case IN_GAME:
    // In-game rendering handled by Game class
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

  // Title box (jak w welcome message)
  attron(COLOR_PAIR(COLOR_MENU_TITLE));
  mvprintw(centerY - 4, centerX - 12, "╔══════════════════════╗");
  mvprintw(centerY - 3, centerX - 12, "║      TRON GAME       ║");
  mvprintw(centerY - 2, centerX - 12, "╠══════════════════════╣");
  attroff(COLOR_PAIR(COLOR_MENU_TITLE));

  // Menu options w tym samym box-ie - manualne formatowanie
  attron(COLOR_PAIR(COLOR_MENU_TEXT));

  // Start Game
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

  // Game Mode
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

  // Settings
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

  // Quit
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
    int maxOptions = 0; // Owiń w nawiasy klamrowe
    if (currentState == MAIN_MENU)
      maxOptions = mainMenuOptions.size();
    else if (currentState == GAME_MODE_MENU)
      maxOptions = gameModeOptions.size();
    else if (currentState == SETTINGS_MENU)
      maxOptions = settingsOptions.size();

    if (selectedOption < maxOptions - 1)
    {
      selectedOption++;
    }
    break;
  }
  case '\n': // Enter key
  case ' ':
    // Handle menu selection
    if (currentState == MAIN_MENU)
    {
      if (selectedOption == 1)
      { // Game Mode
        currentState = GAME_MODE_MENU;
        resetSelection();
      }
      else if (selectedOption == 2)
      { // Settings
        currentState = SETTINGS_MENU;
        resetSelection();
      }
    }
    else if (currentState == GAME_MODE_MENU)
    {
      if (selectedOption == 0)
      { // Single Player - start game
        currentState = MAIN_MENU;
        selectedOption = 0; // Set to Start Game
        resetSelection();
      }
      else if (selectedOption == 3)
      { // Back
        currentState = MAIN_MENU;
        selectedOption = 1; // Set back to Game Mode option
      }
    }
    else if (currentState == SETTINGS_MENU)
    {
      if (selectedOption == 3)
      { // Back
        currentState = MAIN_MENU;
        selectedOption = 2; // Set back to Settings option
      }
    }
    return true; // Option selected
  case 'q':      // Quit
  case 'Q':
  case 27:                    // Escape key
    currentState = MAIN_MENU; // Return to main menu
    selectedOption = 3;       // Reset selection
    return true;
  }
  return false; // No action taken
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
  selectedOption = 0; // Reset to first option
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
  // USUŃ całą pętlę for i ZAMIEŃ na:

  // Single Player
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

  // Two Player (future)
  if (selectedOption == 1)
  {
    attron(COLOR_PAIR(COLOR_MENU_SELECTED));
    mvprintw(centerY, centerX - 12, "║ > Two Player (future)║");
    attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
  }
  else
  {
    mvprintw(centerY, centerX - 12, "║   Two Player (future)║");
  }

  // vs Bot (future)
  if (selectedOption == 2)
  {
    attron(COLOR_PAIR(COLOR_MENU_SELECTED));
    mvprintw(centerY + 1, centerX - 12, "║ > vs Bot (future)    ║");
    attroff(COLOR_PAIR(COLOR_MENU_SELECTED));
  }
  else
  {
    mvprintw(centerY + 1, centerX - 12, "║   vs Bot (future)    ║");
  }

  // Back
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

  // Game Speed
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

  // Difficulty
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

  // Colors
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

  // Back
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