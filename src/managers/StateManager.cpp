#include "../../include/managers/StateManager.h"
#include "../../include/states/GameState.h"

void StateManager::pushState(std::unique_ptr<GameState> state)
{
  if (state)
  {
    state->enter();
    states.push(std::move(state));
  }
}

void StateManager::popState()
{
  if (!states.empty())
  {
    states.top()->exit();
    states.pop();
  }
}

void StateManager::changeState(std::unique_ptr<GameState> state)
{
  if (!states.empty())
  {
    states.top()->exit();
    states.pop();
  }
  pushState(std::move(state));
}

void StateManager::handleInput(InputManager &input)
{
  if (!states.empty())
  {
    states.top()->handleInput(input);
  }
}

void StateManager::update(float deltaTime)
{
  if (!states.empty())
  {
    states.top()->update(deltaTime);
  }
}

void StateManager::render(Renderer &renderer)
{
  if (!states.empty())
  {
    states.top()->render(renderer);
  }
}

bool StateManager::hasStates() const
{
  return !states.empty();
}

GameState *StateManager::getCurrentState() const
{
  return states.empty() ? nullptr : states.top().get();
}