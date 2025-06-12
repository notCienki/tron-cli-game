#include "../include/game.h"
#include "../include/player.h"
#include <unistd.h> // for usleep
#include <cstdio>

Game::Game(int w, int h) : width(w), height(h), running(false), state(PLAYING), firstStart(true), score(0) {}

Game::~Game() {
    cleanup();
}

void Game::init() {
    setlocale(LC_ALL, "");
    
    printf("\033[?1049h\033[H");
    fflush(stdout);
    
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    raw();
    scrollok(stdscr, FALSE);
    
    int termHeight, termWidth;
    getmaxyx(stdscr, termHeight, termWidth);
    height = termHeight;
    width = termWidth;
    running = true;
    startGame();
}

void Game::startGame(){
    gameStartTime = std::chrono::steady_clock::now();
    score = 0;
    state = PLAYING;

    if(firstStart){
        showWelcomeMessage();
        firstStart = false;
    }
}

void Game::showWelcomeMessage() {
    clear();
    drawBorders();
    
    int centerX = width / 2;
    int centerY = height / 2;
    
    mvprintw(centerY - 3, centerX - 12, "╔══════════════════════╗");
    mvprintw(centerY - 2, centerX - 12, "║      TRON GAME       ║");
    mvprintw(centerY - 1, centerX - 12, "╠══════════════════════╣");
    mvprintw(centerY,     centerX - 12, "║   Use ⇠⇡⇢⇣ to move   ║");
    mvprintw(centerY + 1, centerX - 12, "║ Avoid walls & trails ║");
    mvprintw(centerY + 2, centerX - 12, "╚══════════════════════╝");
    
    refresh();
    sleep(2);
}

void Game::updateScore() {
    if(state == PLAYING) {
        currentTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - gameStartTime);
        score = duration.count();
    }
}

int Game::getGameTime() const {
    if(state == PLAYING) {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - gameStartTime);
        return static_cast<int>(duration.count());
    }
    else{
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - gameStartTime);
        return static_cast<int>(duration.count());
    }
}

void Game::run() {
    int actualWidth, actualHeight;
    getmaxyx(stdscr, actualHeight, actualWidth);
    Player player (actualWidth / 2, actualHeight / 2);

    while (running) {
        handleInput(player);

        /*if(state == PLAYING && player.getTrail().empty()) {
            // If the game is in PLAYING state and player has moved
            player.reset(); // Reset player position if needed
        }*/


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
        if (state == GAME_OVER){ 
            restart();
            player.reset(); // Reset player position and direction
        }
        break;
      case 'q':
      case 'Q':
      case 27: // ESC key
        stop(); // Stop the game
        break;
    }
}

void Game::update(Player &player) {
    if (state != PLAYING) return;

    updateScore();

    int nextX = player.getNextX();
    int nextY = player.getNextY();

    if (checkWallCollision (nextX, nextY)){
        gameOver();
        return;
    }

    if (checkTrailCollision(nextX, nextY, player)) {
        gameOver();
        return;
    }

    player.move();
}

void Game::render(Player &player) {
    clear();

    drawBorders(); // Draw the game borders

    // TOP HUD
    mvprintw(0, 3, "╣ Score: %d ║ Time: %ds ╠", score, getGameTime());
    // BOTTOM HUD
    int bottomY = height - 1;
    mvprintw(bottomY, 3, "╣ ⇠⇡⇢⇣ Move ║ Q Quit ║ R Restart ╠");


    // GAME CONTENT
    if (state == GAME_OVER) {
        int centerX = width / 2;
        int centerY = height / 2;
        
        mvprintw(centerY - 2, centerX - 6, "GAME OVER!");
        mvprintw(centerY - 1, centerX - 8, "Final Score: %d", score);
        mvprintw(centerY, centerX - 8, "Survival Time: %ds", getGameTime());
        mvprintw(centerY + 2, centerX - 8, "Press R to restart");
        mvprintw(centerY + 3, centerX - 7, "Press Q to quit");
    } else {
        player.draw();
    }
    
    refresh();
}

void Game::cleanup() {
    endwin();

    printf("\033[?1049l");
    fflush(stdout);
}

bool Game::checkWallCollision(int x, int y) {
    // Check if the player is out of bounds
    return (x <= 0 || x >= width - 1 || y <= 0 || y >= height - 1);
}

void Game::gameOver() {
    state = GAME_OVER;
}

void Game::restart(){
    startGame();
}

bool Game::checkTrailCollision(int x, int y, const Player &player) {
    const auto& trail = player.getTrail();  // Zmień getTrial na getTrail!
    
    if(trail.size() <= 2){
        return false;
    }

    // TrailSegment ma .x i .y, nie .first i .second!
    for (size_t i = 0; i < trail.size() - 2; i++) {
        if (trail[i].x == x && trail[i].y == y) {  // .x i .y zamiast .first i .second
            return true;
        }
    }
    
    return false;
}

void Game::drawBorders() {
    mvprintw(0, 0, "╔");
    for (int x = 1; x < width - 1; x++) {
        mvprintw(0, x, "═");
    }
    mvprintw(0, width - 1, "╗");
    
    // Side borders
    for (int y = 1; y < height - 1; y++) {
        mvprintw(y, 0, "║");
        mvprintw(y, width - 1, "║");
    }
    
    // Bottom border
    mvprintw(height - 1, 0, "╚");
    for (int x = 1; x < width - 1; x++) {
        mvprintw(height - 1, x, "═");
    }
    mvprintw(height - 1, width - 1, "╝");
}

int Game::getScore() const {
    return score;
}

