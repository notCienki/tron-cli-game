#include "../include/game.h"
#include "../include/player.h"
#include <unistd.h> // for usleep

Game::Game(int w, int h) : width(w), height(h), running(false) {}

Game::~Game() {
    cleanup();
}

void Game::init() {
    initscr(); // Initialize ncurses
    noecho(); // Don't echo input characters
    curs_set(0); // Hide the cursor
    keypad(stdscr, TRUE); // Enable special keys (like arrow keys)
    nodelay(stdscr, TRUE); // Don't block on input

    border('|', '|', '-', '-', '+', '+', '+', '+'); // Draw a border
    running = true; // Set the game to running state
}

void Game::run() {
  Player player (width / 2, height / 2, '*'); // Create a player at the center of the screen

    while (running) {
        handleInput(player);
        update(player);
        render(player);
        usleep(100000); // Sleep for 100 milliseconds to control the game speed
    }
}

void Game::handleInput(Player &player) {
    int ch = getch(); // Get user input
    switch (ch) {
      case KEY_UP:
            player.setDirection(UP);
            break;
        case KEY_DOWN:
            player.setDirection(DOWN);
            break;
        case KEY_LEFT:
            player.setDirection(LEFT);
            break;
        case KEY_RIGHT:
            player.setDirection(RIGHT);
            break;
      case 'q':
      case 'Q':
      case 27: // ESC key
        stop(); // Stop the game
        break;
    }
}

void Game::update(Player &player) {
  player.move(); // Move the player based on current direction
  // Update game logic here (e.g., player movement, collision detection)
}

void Game::render(Player &player) {
  clear();
  border('|', '|', '-', '-', '+', '+', '+', '+');
  player.draw(); // Draw the player on the screen
  refresh(); // Refresh the screen to show changes
}

void Game::cleanup() {
    endwin(); // End ncurses mode
}