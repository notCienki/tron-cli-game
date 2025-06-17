#pragma once

#include "GameMode.h"
#include "../../entities/bot.h"

class SinglePlayerMode : public GameModeStrategy
{
private:
  std::chrono::steady_clock::time_point gameStartTime;

public:
  explicit SinglePlayerMode(std::shared_ptr<GameConfig> config);

  void handleInput(InputManager &inputManager, PlayState currentState) override;
  void update() override;
  std::string getModeName() const override { return "Single Player"; }
  void reset() override;
  void initializePlayers() override;

  std::pair<int, int> getRandomPositionOnSide(int side, int width, int height);
  Direction getSafeDirection(int side);
};