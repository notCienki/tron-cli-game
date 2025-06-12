#include "../include/player.h"
#include <ncurses.h>

Player::Player(int startX, int startY, char sym) 
    : x(startX), y(startY), startX(startX), startY(startY), direction(RIGHT), symbol(sym) {
    trial.push_back({x, y}); // Add starting position to the trial
}

void Player::move() {
    switch (direction) {
        case UP:    y--; break;
        case DOWN:  y++; break;
        case LEFT:  x--; break;
        case RIGHT: x++; break;
    }

    trial.push_back({x, y}); // Add new position to the trial
}

void Player::setDirection(Direction newDir) {
    // Prevent reversing direction
    if ((direction == UP && newDir == DOWN) ||
        (direction == DOWN && newDir == UP) ||
        (direction == LEFT && newDir == RIGHT) ||
        (direction == RIGHT && newDir == LEFT)) {
        return; // Don't allow reversing
    }

    direction = newDir; // Update direction
}

void Player::draw() {
    for(const auto& pos : trial) {
        mvaddch(pos.second, pos.first, symbol); // Draw the player at each position in the trial
    }
}

int Player::getNextX() const {
    switch (direction) {
        case LEFT:  return x - 1;
        case RIGHT: return x + 1;
        default: return x;
    }   
}

int Player::getNextY() const {
    switch(direction) {
        case UP:    return y - 1;
        case DOWN:  return y + 1;
        default: return y;
    }
}

void Player::reset() {
    x = startX; // Reset to starting position
    y = startY;
    direction = RIGHT; // Reset direction to default
    trial.clear(); // Clear the trial history
    trial.push_back({x, y}); // Add starting position back to the trial
}