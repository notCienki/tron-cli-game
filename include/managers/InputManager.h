#pragma once
#include <queue>
#include "../core/types.h"

class InputManager
{
private:
  std::queue<int> inputQueue;

public:
  void pollInput();
  bool hasInput() const;
  int getNextInput();
  void clearInput();

  static bool isMovementKey(int key);
  static bool isActionKey(int key);

  Direction keyToDirection(int key);

  static bool isPlayer1Key(int key);
  static bool isPlayer2Key(int key);
};