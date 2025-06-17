#pragma once

#include "../managers/InputManager.h"
#include "../core/GameConfig.h"
#include "../rendering/Renderer.h"
#include <memory>
#include <unordered_map>
#include <string>

class GameState;
class StateManager;
class InputManager;
class Renderer;
class GameConfig;

class GameEngine
{
private:
  std::unique_ptr<StateManager> stateManager;
  std::shared_ptr<InputManager> inputManager;
  std::shared_ptr<Renderer> renderer;
  std::shared_ptr<GameConfig> gameConfig;

  bool running = true;

public:
  GameEngine();
  ~GameEngine();

  void initialize();
  void run();
  void shutdown();

  // Dependency access for states
  InputManager &getInputManager() { return *inputManager; }
  Renderer &getRenderer() { return *renderer; }
  GameConfig &getGameConfig() { return *gameConfig; }
};