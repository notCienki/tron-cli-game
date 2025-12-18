#pragma once

namespace Config
{
  const int SPAWN_MARGIN = 10;
  const int COLLISION_TRAIL_MIN_LENGTH = 2;

  const int WELCOME_MESSAGE_DELAY_SEC = 2;

  const int COLOR_PLAYER_HEAD = 1;
  const int COLOR_PLAYER_TRAIL = 2;

  const int COLOR_PLAYER2_HEAD = 9;
  const int COLOR_PLAYER2_TRAIL = 10;

  const int COLOR_BORDERS = 3;
  const int COLOR_GAME_OVER = 4;
  const int COLOR_HUD = 5;
  const int COLOR_MESSAGES = 6;

  const int COLOR_MENU_BORDER = 11;
  const int COLOR_MENU_TEXT = 12;
  const int COLOR_MENU_SELECTED = 13;
  const int COLOR_MENU_TITLE = 14;

  const int MENU_BOX_HALF_WIDTH = 12;
  const int MENU_BOX_LARGE_HALF_WIDTH = 15;

  const int HUD_HORIZONTAL_OFFSET = 3;

  const int GAMEOVER_BOX_VERTICAL_OFFSET = 3;
  const int WELCOME_BOX_VERTICAL_OFFSET = 3;

  const int SIDE_TOP = 0;
  const int SIDE_RIGHT = 1;
  const int SIDE_BOTTOM = 2;
  const int SIDE_LEFT = 3;
  const int NUM_SIDES = 4;

  const int PLAYER_1_ID = 1;
  const int PLAYER_2_ID = 2;

  const int WINNER_TIE = 0;
  const int WINNER_PLAYER1 = 1;
  const int WINNER_PLAYER2 = 2;

  const int DEFAULT_BOT_DIFFICULTY = 1;
}
