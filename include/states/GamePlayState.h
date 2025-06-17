#pragma once

#include "GameState.h"
#include "../core/types.h"
#include "../states/modes/GameMode.h"
#include <memory>

class GamePlayState : public GameState
{
private:
  std::unique_ptr<GameModeStrategy> gameMode;
  GameSpeed gameSpeed;
  int colorScheme;
  bool shouldReturnToMenu = false;

public:
  GamePlayState(GameMode mode, GameSpeed speed, int colors);
  ~GamePlayState() override = default;

  void enter() override;
  void exit() override;

  void handleInput(InputManager &input) override;
  void update(float deltaTime) override;
  void render(Renderer &renderer) override;

  bool wantsToReturnToMenu() const { return shouldReturnToMenu; }
};