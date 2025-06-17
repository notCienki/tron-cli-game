#include "../../include/core/GameEngine.h"
#include "../../include/managers/StateManager.h"
#include "../../include/states/MenuState.h"
#include "../../include/states/GamePlayState.h"
#include <ncurses.h>

GameEngine::GameEngine()
{
  gameConfig = std::make_shared<GameConfig>();
  inputManager = std::make_shared<InputManager>();
  renderer = std::make_shared<Renderer>(gameConfig);
  stateManager = std::make_unique<StateManager>();
}

GameEngine::~GameEngine()
{
  shutdown();
}

void GameEngine::initialize()
{
  // Initialize ncurses (przenieś z main.cpp)
  setlocale(LC_ALL, "");
  printf("\033[?1049h\033[H");
  fflush(stdout);

  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, FALSE);

  // Initialize renderer
  renderer->initialize();

  // Push initial state
  auto menuState = std::make_unique<MenuState>();
  stateManager->pushState(std::move(menuState));
}

void GameEngine::run()
{
  const float targetFPS = 60.0f;
  const float deltaTime = 1.0f / targetFPS;

  while (running && stateManager->hasStates())
  {
    stateManager->handleInput(*inputManager);
    stateManager->update(deltaTime);
    stateManager->render(*renderer);

    // Check for state transitions
    auto *currentState = stateManager->getCurrentState();
    if (auto *menuState = dynamic_cast<MenuState *>(currentState))
    {
      if (menuState->wantsToStartGame())
      {
        // Create GamePlayState with selected settings
        auto gameState = std::make_unique<GamePlayState>(
            menuState->getSelectedGameMode(),
            menuState->getSelectedGameSpeed(),
            menuState->getSelectedColorScheme());
        stateManager->changeState(std::move(gameState));
      }
      else if (menuState->wantsToQuit())
      {
        running = false;
      }
    }

    usleep(16667); // ~60 FPS
  }
}

void GameEngine::shutdown()
{
  while (stateManager && stateManager->hasStates())
  {
    stateManager->popState();
  }

  if (renderer)
  {
    renderer->cleanup();
  }

  endwin();
}