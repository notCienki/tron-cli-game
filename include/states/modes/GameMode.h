#pragma once

#include <vector>
#include <memory>
#include "../../core/types.h"
#include "../../entities/player.h"

class InputManager;
class Renderer;
class GameConfig;

class GameModeStrategy
{
protected:
  std::vector<std::unique_ptr<Player>> players;
  std::shared_ptr<GameConfig> config;
  bool gameFinished = false;
  int winner = 0;

public:
  explicit GameModeStrategy(std::shared_ptr<GameConfig> config);
  virtual ~GameModeStrategy() = default;

  virtual void handleInput(InputManager &inputManager, PlayState currentState) = 0;
  virtual void update() = 0;
  virtual std::string getModeName() const = 0;

  virtual bool isGameFinished() const { return gameFinished; }
  virtual int getWinner() const { return winner; }
  virtual void reset() = 0;

  std::vector<Player *> getPlayersForRendering() const;
  virtual void initializePlayers() = 0;
};