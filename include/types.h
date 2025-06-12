#pragma once

enum GameState
{
  PLAYING,
  GAME_OVER,
  PAUSED
};

// Common game types and enums
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
  SLOW = 150000,   // 150ms
  NORMAL = 100000, // 100ms
  FAST = 50000     // 50ms
};

// Application states
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
  IN_GAME
};