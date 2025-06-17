#include "../../include/rendering/Renderer.h"
#include "../../include/entities/player.h"
#include <iostream>

Renderer::Renderer(std::shared_ptr<GameConfig> config)
    : config(config), screenWidth(0), screenHeight(0)
{
  updateScreenSize();
}

void Renderer::initialize()
{
  setlocale(LC_ALL, "");
  printf("\033[?1049h\033[H");
  fflush(stdout);

  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  raw();
  scrollok(stdscr, FALSE);

  initializeColors();
  updateScreenSize();
}

void Renderer::clear()
{
  ::clear();
}

void Renderer::refresh()
{
  ::refresh();
}

void Renderer::cleanup()
{
  endwin();
  printf("\033[?1049l");
  fflush(stdout);
}

void Renderer::updateScreenSize()
{
  getmaxyx(stdscr, screenHeight, screenWidth);
}

void Renderer::initializeColors()
{
  if (has_colors())
  {
    start_color();
    use_default_colors();

    switch (config->colorScheme)
    {
    case 0:
      init_pair(COLOR_PLAYER_HEAD, COLOR_CYAN, -1);
      init_pair(COLOR_PLAYER_TRAIL, COLOR_BLUE, -1);
      init_pair(COLOR_PLAYER2_HEAD, COLOR_RED, -1);
      init_pair(COLOR_PLAYER2_TRAIL, COLOR_YELLOW, -1);
      init_pair(COLOR_BORDERS, COLOR_WHITE, -1);
      break;
    case 1:
      init_pair(COLOR_PLAYER_HEAD, COLOR_MAGENTA, -1);
      init_pair(COLOR_PLAYER_TRAIL, COLOR_YELLOW, -1);
      init_pair(COLOR_PLAYER2_HEAD, COLOR_GREEN, -1);
      init_pair(COLOR_PLAYER2_TRAIL, COLOR_CYAN, -1);
      init_pair(COLOR_BORDERS, COLOR_RED, -1);
      break;
    case 2:
      init_pair(COLOR_PLAYER_HEAD, COLOR_GREEN, -1);
      init_pair(COLOR_PLAYER_TRAIL, COLOR_WHITE, -1);
      init_pair(COLOR_PLAYER2_HEAD, COLOR_YELLOW, -1);
      init_pair(COLOR_PLAYER2_TRAIL, COLOR_MAGENTA, -1);
      init_pair(COLOR_BORDERS, COLOR_GREEN, -1);
      break;
    default:
      init_pair(COLOR_PLAYER_HEAD, COLOR_CYAN, -1);
      init_pair(COLOR_PLAYER_TRAIL, COLOR_BLUE, -1);
      init_pair(COLOR_PLAYER2_HEAD, COLOR_RED, -1);
      init_pair(COLOR_PLAYER2_TRAIL, COLOR_YELLOW, -1);
      init_pair(COLOR_BORDERS, COLOR_WHITE, -1);
      break;
    }
    init_pair(COLOR_GAME_OVER, COLOR_RED, -1);
    init_pair(COLOR_HUD, COLOR_YELLOW, -1);
    init_pair(COLOR_MESSAGES, COLOR_MAGENTA, -1);
  }
}

void Renderer::setColorScheme(int scheme)
{
  config->colorScheme = scheme;
  initializeColors();
}

void Renderer::drawBorders()
{
  attron(COLOR_PAIR(COLOR_BORDERS));

  mvprintw(0, 0, "╔");
  for (int x = 1; x < screenWidth - 1; x++)
  {
    mvprintw(0, x, "═");
  }
  mvprintw(0, screenWidth - 1, "╗");

  for (int y = 1; y < screenHeight - 1; y++)
  {
    mvprintw(y, 0, "║");
    mvprintw(y, screenWidth - 1, "║");
  }

  mvprintw(screenHeight - 1, 0, "╚");
  for (int x = 1; x < screenWidth - 1; x++)
  {
    mvprintw(screenHeight - 1, x, "═");
  }
  mvprintw(screenHeight - 1, screenWidth - 1, "╝");

  attroff(COLOR_PAIR(COLOR_BORDERS));
}

void Renderer::drawWelcomeMessage()
{
  int centerX = screenWidth / 2;
  int centerY = screenHeight / 2;

  mvprintw(centerY - 3, centerX - GameConfig::WELCOME_BOX_HALF_WIDTH, "╔══════════════════════╗");
  mvprintw(centerY - 2, centerX - GameConfig::WELCOME_BOX_HALF_WIDTH, "║      TRON GAME       ║");
  mvprintw(centerY - 1, centerX - GameConfig::WELCOME_BOX_HALF_WIDTH, "╠══════════════════════╣");
  mvprintw(centerY, centerX - GameConfig::WELCOME_BOX_HALF_WIDTH, "║   Use ⇠⇡⇢⇣ to move   ║");
  mvprintw(centerY + 1, centerX - GameConfig::WELCOME_BOX_HALF_WIDTH, "║ Avoid walls & trails ║");
  mvprintw(centerY + 2, centerX - GameConfig::WELCOME_BOX_HALF_WIDTH, "╚══════════════════════╝");
}

void Renderer::drawHUD(const std::string &mode, int score, int time)
{
  attron(COLOR_PAIR(COLOR_HUD));
  mvprintw(0, 3, "╣ %s ║ Score: %d ║ Time: %ds ╠", mode.c_str(), score, time);
  int bottomY = screenHeight - 1;
  mvprintw(bottomY, 3, "╣ ⇠⇡⇢⇣ Move ║ Q Quit ║ R Restart ╠");
  attroff(COLOR_PAIR(COLOR_HUD));
}

void Renderer::drawPlayers(const std::vector<Player *> &players)
{
  for (Player *player : players)
  {
    if (player != nullptr)
    {
      player->draw();
    }
  }
}

void Renderer::drawGameOverScreen(int winner, int score, int time, const std::string &mode)
{
  int centerX = screenWidth / 2;
  int centerY = screenHeight / 2;

  attron(COLOR_PAIR(COLOR_GAME_OVER));
  mvprintw(centerY - 3, centerX - 15, "╔═══════════════════════════════╗");

  if (winner == 1)
  {
    mvprintw(centerY - 2, centerX - 15, "║        PLAYER 1 WINS!         ║");
  }
  else if (winner == 2)
  {
    mvprintw(centerY - 2, centerX - 15, "║        PLAYER 2 WINS!         ║");
  }
  else
  {
    mvprintw(centerY - 2, centerX - 15, "║         GAME OVER!            ║");
  }

  mvprintw(centerY - 1, centerX - 15, "╠═══════════════════════════════╣");
  mvprintw(centerY, centerX - 15, "║ Score:%3d   Time:%2ds        ║", score, time);
  mvprintw(centerY + 1, centerX - 15, "╠═══════════════════════════════╣");
  attroff(COLOR_PAIR(COLOR_GAME_OVER));

  attron(COLOR_PAIR(COLOR_MESSAGES));
  mvprintw(centerY + 2, centerX - 15, "║     R-Restart    Q-Quit       ║");
  mvprintw(centerY + 3, centerX - 15, "╚═══════════════════════════════╝");
  attroff(COLOR_PAIR(COLOR_MESSAGES));
}

void Renderer::renderGameScreen(PlayState state, const std::vector<Player *> &players,
                                const std::string &mode, int score, int time, int winner)
{
  clear();
  drawBorders();

  if (state == PLAYING)
  {
    drawPlayers(players);

    drawHUD(mode, score, time);
  }
  else if (state == GAME_OVER)
  {
    drawGameOverScreen(winner, score, time, mode);
  }

  refresh();
}

void Renderer::showWelcome()
{
  clear();
  drawBorders();
  drawWelcomeMessage();
  refresh();
  sleep(GameConfig::WELCOME_MESSAGE_DURATION_SEC);
}