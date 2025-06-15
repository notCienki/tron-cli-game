#pragma once
#include <queue>

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
};