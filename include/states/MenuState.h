#pragma once

#include "GameState.h"
#include "../Menu.h"
#include <memory>

class MenuState : public GameState
{
private:
  std::unique_ptr<Menu> menu;
  bool shouldStartGame = false;
  bool shouldQuit = false;

public:
  MenuState();
  ~MenuState() override = default;

  void enter() override;
  void exit() override;

  void handleInput(InputManager &input) override;
  void update(float deltaTime) override;
  void render(Renderer &renderer) override;

  bool wantsToStartGame() const { return shouldStartGame; }
  bool wantsToQuit() const { return shouldQuit; }
  GameMode getSelectedGameMode() const;
  GameSpeed getSelectedGameSpeed() const;
  int getSelectedColorScheme() const;
};