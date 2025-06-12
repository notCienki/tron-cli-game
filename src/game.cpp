#include "../include/game.h"
#include "../include/player.h"
#include <unistd.h> // for usleep

Game::Game(int w, int h) : width(w), height(h), running(false), state(PLAYING) {}

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
            if (state == PLAYING) player.setDirection(UP);
            break;
        case KEY_DOWN:
            if (state == PLAYING) player.setDirection(DOWN);
            break;
        case KEY_LEFT:
            if (state == PLAYING) player.setDirection(LEFT);
            break;
        case KEY_RIGHT:
            if (state == PLAYING) player.setDirection(RIGHT);
            break;
      case 'r':
      case 'R':
        if (state == GAME_OVER) restart();
        break;
      case 'q':
      case 'Q':
      case 27: // ESC key
        stop(); // Stop the game
        break;
    }
}

void Game::update(Player &player) {
    if (state != PLAYING) return; // Only update if the game is in PLAYING state

    int nextX = player.getNextX();
    int nextY = player.getNextY();

    if (checkWallCollision (nextX, nextY)){
        gameOver();
        return;
    }

    player.move(); // Move the player based on current direction
}

void Game::render(Player &player) {
  clear();
  border('|', '|', '-', '-', '+', '+', '+', '+');
  
  if (state == GAME_OVER) {
    int centerX = width / 2;
    int centerY = height / 2;

    mvprintw(centerY, centerX - 5, "GAME OVER");
    mvprintw(centerY + 1, centerX - 8, "Press R to restart");

  }

  player.draw(); // Draw the player on the screen
  refresh(); // Refresh the screen to show changes
}

void Game::cleanup() {
    endwin(); // End ncurses mode
}

bool Game::checkWallCollision(int x, int y) {
    // Check if the player is out of bounds
    return (x <= 0 || x >= width - 1 || y <= 0 || y >= height - 1);
}

void Game::gameOver() {
    state = GAME_OVER; // Set the game state to GAME_OVER
}

void Game::restart(){
    state = PLAYING; // Reset the game state to PLAYING
}
