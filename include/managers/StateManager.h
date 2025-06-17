#pragma once

#include <memory>
#include <stack>

class GameState;
class InputManager;
class Renderer;
class GameConfig;

class StateManager
{
private:
  std::stack<std::unique_ptr<GameState>> states;

public:
  StateManager() = default;
  ~StateManager() = default;

  void pushState(std::unique_ptr<GameState> state);
  void popState();
  void changeState(std::unique_ptr<GameState> state);

  void handleInput(InputManager &input);
  void update(float deltaTime);
  void render(Renderer &renderer);

  bool hasStates() const;
  GameState *getCurrentState() const;
};