#pragma once

namespace Config
{

  // Spawn and boundary settings
  const int SPAWN_MARGIN = 10;              // Distance from edges for spawn points
  const int COLLISION_TRAIL_MIN_LENGTH = 2; // Min trail length before self-collision

  const int WELCOME_MESSAGE_DELAY_SEC = 2; // Welcome screen display time (seconds)

  // Player 1 colors
  const int COLOR_PLAYER_HEAD = 1;
  const int COLOR_PLAYER_TRAIL = 2;

  // Player 2 colors
  const int COLOR_PLAYER2_HEAD = 9;
  const int COLOR_PLAYER2_TRAIL = 10;

  // Game UI colors
  const int COLOR_BORDERS = 3;
  const int COLOR_GAME_OVER = 4;
  const int COLOR_HUD = 5;
  const int COLOR_MESSAGES = 6;

  // Menu colors
  const int COLOR_MENU_BORDER = 11;
  const int COLOR_MENU_TEXT = 12;
  const int COLOR_MENU_SELECTED = 13;
  const int COLOR_MENU_TITLE = 14;

  // Menu box dimensions and positioning
  const int MENU_BOX_HALF_WIDTH = 12;       // Half-width of menu boxes for centering
  const int MENU_BOX_LARGE_HALF_WIDTH = 15; // Half-width for larger boxes (game over)

  // HUD positioning offsets
  const int HUD_HORIZONTAL_OFFSET = 3; // Space from left edge for HUD elements

  // Game over / message box positioning offsets
  const int GAMEOVER_BOX_VERTICAL_OFFSET = 3; // Vertical offset for game over box
  const int WELCOME_BOX_VERTICAL_OFFSET = 3;  // Vertical offset for welcome message

  // Side IDs for spawn positioning (used in getRandomPositionOnSide)
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

  const int DEFAULT_BOT_DIFFICULTY = 1; // Medium difficulty
}
