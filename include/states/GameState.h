#pragma once

class InputManager;
class Renderer;
class GameConfig;

class GameState
{
public:
  virtual ~GameState() = default;

  // Lifecycle
  virtual void enter() {}
  virtual void exit() {}

  // Game loop
  virtual void handleInput(InputManager &input) = 0;
  virtual void update(float deltaTime) = 0;
  virtual void render(Renderer &renderer) = 0;

  // State transitions
  virtual bool shouldExit() const { return false; }
};