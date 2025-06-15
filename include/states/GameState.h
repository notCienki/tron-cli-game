#pragma once
#include <string>

class GameState
{
public:
  virtual ~GameState() = default;

  virtual void enter() {}
  virtual void exit() {}
  virtual void handleInput(int key) = 0;
  virtual void update(float deltaTime) = 0;
  virtual void render() = 0;

  virtual bool shouldTransition() const { return false; }
  virtual std::string getNextState() const { return ""; }
};