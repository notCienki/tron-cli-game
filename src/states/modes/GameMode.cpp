#include "../../../include/states/modes/GameMode.h"

GameModeStrategy::GameModeStrategy(std::shared_ptr<GameConfig> config) : config(config) {}

std::vector<Player *> GameModeStrategy::getPlayersForRendering() const
{
  std::vector<Player *> result;
  for (const auto &player : players)
  {
    result.push_back(player.get());
  }
  return result;
}