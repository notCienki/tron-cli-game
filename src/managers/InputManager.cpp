#include "../../include/managers/InputManager.h"
#include <ncurses.h>

void InputManager::pollInput()
{
  int key = getch();
  if (key != ERR)
  {
    inputQueue.push(key);
  }
}

bool InputManager::hasInput() const
{
  return !inputQueue.empty();
}

int InputManager::getNextInput()
{
  if (inputQueue.empty())
    return ERR;

  int key = inputQueue.front();
  inputQueue.pop();
  return key;
}

void InputManager::clearInput()
{
  while (!inputQueue.empty())
  {
    inputQueue.pop();
  }
}

bool InputManager::isMovementKey(int key)
{
  return key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT || key == KEY_RIGHT ||
         key == 'w' || key == 'a' || key == 's' || key == 'd' ||
         key == 'W' || key == 'A' || key == 'S' || key == 'D';
}

bool InputManager::isActionKey(int key)
{
  return (key == 'r' || key == 'R' ||
          key == 'q' || key == 'Q' ||
          key == 27 ||
          key == ' ' || key == '\n');
}

Direction InputManager::keyToDirection(int key)
{
  switch (key)
  {
  case KEY_UP:
  case 'w':
  case 'W':
    return UP;
  case KEY_DOWN:
  case 's':
  case 'S':
    return DOWN;
  case KEY_LEFT:
  case 'a':
  case 'A':
    return LEFT;
  case KEY_RIGHT:
  case 'd':
  case 'D':
    return RIGHT;
  default:
    return UP;
  }
}

bool InputManager::isPlayer1Key(int key)
{
  return (key == KEY_UP || key == KEY_DOWN ||
          key == KEY_LEFT || key == KEY_RIGHT);
}

bool InputManager::isPlayer2Key(int key)
{
  return (key == 'w' || key == 'W' || key == 's' || key == 'S' ||
          key == 'a' || key == 'A' || key == 'd' || key == 'D');
}