#pragma once

enum GameState
{
  PLAYING,
  GAME_OVER,
  PAUSED
};

enum GameMode
{
  SINGLE_PLAYER = 0,
  TWO_PLAYER = 1,
  VS_BOT = 2
};

enum GameDifficulty
{
  EASY = 0,
  MEDIUM = 1,
  HARD = 2
};

enum GameSpeed
{
  SLOW = 150000,
  NORMAL = 100000,
  FAST = 50000
};

enum AppState
{
  STATE_MENU,
  STATE_GAME,
  STATE_SETTINGS,
  STATE_QUIT
};

enum Direction
{
  UP = 0,
  DOWN = 1,
  LEFT = 2,
  RIGHT = 3
};

enum MenuState
{
  MAIN_MENU,
  GAME_MODE_MENU,
  SETTINGS_MENU,
  GAME_SPEED_MENU,
  DIFFICULTY_MENU,
  COLOR_SCHEME_MENU,
  IN_GAME
};