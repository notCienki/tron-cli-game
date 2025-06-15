#pragma once

#include "types.h"
class GameConfig
{
public:
  static constexpr int SPAWN_MARGIN = 10;
  static constexpr int SPAWN_SIDES_COUNT = 4;

  static constexpr int MIN_TRAIL_LENGTH_FOR_COLLISION = 2;

  static constexpr int WELCOME_MESSAGE_DURATION_SEC = 2;
  static constexpr int WELCOME_BOX_HALF_WIDTH = 12;
  static constexpr int WELCOME_BOX_LINES = 6;

  GameSpeed currentGameSpeed = NORMAL;
  int colorScheme = 0;
  GameMode gameMode = SINGLE_PLAYER;
};
