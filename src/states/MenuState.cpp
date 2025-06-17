#include "../../include/states/MenuState.h"
#include "../../include/managers/InputManager.h"
#include "../../include/rendering/Renderer.h"

MenuState::MenuState() : menu(std::make_unique<Menu>())
{
}

void MenuState::enter()
{
  menu->init();
  shouldStartGame = false;
  shouldQuit = false;
}

void MenuState::exit()
{
  // cleanup if needed
}

void MenuState::handleInput(InputManager &input)
{
  input.pollInput();

  while (input.hasInput())
  {
    int key = input.getNextInput();

    // Przekieruj do Menu (możesz zaadaptować istniejący Menu::handleInput)
    if (menu->handleInput())
    {
      if (menu->shouldStartGame())
      {
        shouldStartGame = true;
      }
      if (menu->shouldQuit())
      {
        shouldQuit = true;
      }
    }
  }
}

void MenuState::update(float deltaTime)
{
  // Menu nie potrzebuje update'u, ale możesz dodać animacje
}

void MenuState::render(Renderer &renderer)
{
  menu->render();
}

GameMode MenuState::getSelectedGameMode() const
{
  return menu->getGameMode();
}

GameSpeed MenuState::getSelectedGameSpeed() const
{
  return menu->getGameSpeed();
}

int MenuState::getSelectedColorScheme() const
{
  return menu->getColorScheme();
}