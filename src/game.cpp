#include "../include/game.h"
#include "../include/player.h"
#include <unistd.h> // for usleep
#include <cstdio>
#include <random>
#include <ctime>

Game::Game(int w, int h) : width(w), height(h), running(false), state(PLAYING), currentGameSpeed(NORMAL), currentGameMode(SINGLE_PLAYER), currentColorScheme(0), firstStart(true), score(0), winner(0) {}

Game::~Game()
{
    cleanup();
}

void Game::init()
{
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

    initColors();

    int termHeight, termWidth;
    getmaxyx(stdscr, termHeight, termWidth);
    height = termHeight;
    width = termWidth;
    running = true;
    startGame();
}

void Game::startGame()
{
    gameStartTime = std::chrono::steady_clock::now();
    score = 0;
    state = PLAYING;

    if (firstStart)
    {
        showWelcomeMessage();
        firstStart = false;
    }
}

void Game::showWelcomeMessage()
{
    clear();
    drawBorders();

    int centerX = width / 2;
    int centerY = height / 2;

    mvprintw(centerY - 3, centerX - 12, "╔══════════════════════╗");
    mvprintw(centerY - 2, centerX - 12, "║      TRON GAME       ║");
    mvprintw(centerY - 1, centerX - 12, "╠══════════════════════╣");
    mvprintw(centerY, centerX - 12, "║   Use ⇠⇡⇢⇣ to move   ║");
    mvprintw(centerY + 1, centerX - 12, "║ Avoid walls & trails ║");
    mvprintw(centerY + 2, centerX - 12, "╚══════════════════════╝");

    refresh();
    sleep(2);
}

void Game::updateScore()
{
    if (state == PLAYING)
    {
        currentTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - gameStartTime);
        score = duration.count();
    }
}

int Game::getGameTime() const
{
    if (state == PLAYING)
    {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - gameStartTime);
        return static_cast<int>(duration.count());
    }
    else
    {
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - gameStartTime);
        return static_cast<int>(duration.count());
    }
}

void Game::run()
{
    int actualWidth, actualHeight;
    getmaxyx(stdscr, actualHeight, actualWidth);

    if (currentGameMode == SINGLE_PLAYER)
    {
        // SINGLE PLAYER MODE
        auto spawnPos = getRandomSpawnPosition(actualWidth, actualHeight);
        Player player(spawnPos.first, spawnPos.second);

        while (running)
        {
            handleInput(player);
            update(player);
            render(player);
            usleep(currentGameSpeed);
        }
    }
    else if (currentGameMode == TWO_PLAYER)
    {
        // TWO PLAYER MODE
        auto spawnPositions = getTwoPlayerSpawnPositions(actualWidth, actualHeight);
        Player player1(spawnPositions.first.first, spawnPositions.first.second, 1);   // Player 1
        Player player2(spawnPositions.second.first, spawnPositions.second.second, 2); // Player 2

        while (running)
        {
            handleInputTwoPlayer(player1, player2); // New method needed
            updateTwoPlayer(player1, player2);      // New method needed
            renderTwoPlayer(player1, player2);      // New method needed
            usleep(currentGameSpeed);
        }
    }
}

void Game::handleInput(Player &player)
{
    int ch = getch(); // Get user input
    switch (ch)
    {
    case KEY_UP:
        if (state == PLAYING)
            player.setDirection(UP);
        break;
    case KEY_DOWN:
        if (state == PLAYING)
            player.setDirection(DOWN);
        break;
    case KEY_LEFT:
        if (state == PLAYING)
            player.setDirection(LEFT);
        break;
    case KEY_RIGHT:
        if (state == PLAYING)
            player.setDirection(RIGHT);
        break;
    case 'r':
    case 'R':
        if (state == GAME_OVER)
        {
            restart(player);
        }
        break;
    case 'q':
    case 'Q':
    case 27:    // ESC key
        stop(); // Stop the game
        break;
    }
}

void Game::update(Player &player)
{
    if (state != PLAYING)
        return;

    updateScore();

    int nextX = player.getNextX();
    int nextY = player.getNextY();

    if (checkWallCollision(nextX, nextY))
    {
        gameOver();
        return;
    }

    if (checkTrailCollision(nextX, nextY, player))
    {
        gameOver();
        return;
    }

    player.move();
}

void Game::render(Player &player)
{
    clear();

    drawBorders(); // Draw the game borders
    if (state == PLAYING)
    {
        player.draw();

        // TOP HUD
        attron(COLOR_PAIR(COLOR_HUD));
        mvprintw(0, 3, "╣ Score: %d ║ Time: %ds ╠", score, getGameTime());
        // BOTTOM HUD
        int bottomY = height - 1;
        mvprintw(bottomY, 3, "╣ ⇠⇡⇢⇣ Move ║ Q Quit ║ R Restart ╠");
        attroff(COLOR_PAIR(COLOR_HUD));
    }
    else if (state == GAME_OVER)
    {
        int centerX = width / 2;
        int centerY = height / 2;

        // Game Over box podobny do welcome message
        attron(COLOR_PAIR(COLOR_GAME_OVER));
        mvprintw(centerY - 3, centerX - 12, "╔══════════════════════╗");
        mvprintw(centerY - 2, centerX - 12, "║      GAME OVER!      ║");
        mvprintw(centerY - 1, centerX - 12, "╠══════════════════════╣");
        mvprintw(centerY, centerX - 12, "║ Score:%3d   Time:%2ds ║", score, getGameTime());
        mvprintw(centerY + 1, centerX - 12, "╠══════════════════════╣");
        attroff(COLOR_PAIR(COLOR_GAME_OVER));

        attron(COLOR_PAIR(COLOR_MESSAGES));
        mvprintw(centerY + 2, centerX - 12, "║   R-Restart  Q-Quit  ║");
        mvprintw(centerY + 3, centerX - 12, "╚══════════════════════╝");
        attroff(COLOR_PAIR(COLOR_MESSAGES));
    }

    refresh();
}

void Game::cleanup()
{
    endwin();

    printf("\033[?1049l");
    fflush(stdout);
}

bool Game::checkWallCollision(int x, int y)
{
    // Check if the player is out of bounds
    return (x <= 0 || x >= width - 1 || y <= 0 || y >= height - 1);
}

void Game::gameOver()
{
    state = GAME_OVER;
}

void Game::restart(Player &player)
{
    // Generate new random spawn position
    int actualWidth, actualHeight;
    getmaxyx(stdscr, actualHeight, actualWidth);
    auto spawnPos = getRandomSpawnPosition(actualWidth, actualHeight);

    // Reset game state
    startGame();

    // Reset player with new position
    player.reset(spawnPos.first, spawnPos.second);
}

bool Game::checkTrailCollision(int x, int y, const Player &player)
{
    const auto &trail = player.getTrail();

    if (trail.size() <= 2)
    {
        return false;
    }

    // TrailSegment ma .x i .y, nie .first i .second!
    for (size_t i = 0; i < trail.size() - 2; i++)
    {
        if (trail[i].x == x && trail[i].y == y)
        { // .x i .y zamiast .first i .second
            return true;
        }
    }

    return false;
}

void Game::drawBorders()
{
    attron(COLOR_PAIR(COLOR_BORDERS));

    mvprintw(0, 0, "╔");
    for (int x = 1; x < width - 1; x++)
    {
        mvprintw(0, x, "═");
    }
    mvprintw(0, width - 1, "╗");

    // Side borders
    for (int y = 1; y < height - 1; y++)
    {
        mvprintw(y, 0, "║");
        mvprintw(y, width - 1, "║");
    }

    // Bottom border
    mvprintw(height - 1, 0, "╚");
    for (int x = 1; x < width - 1; x++)
    {
        mvprintw(height - 1, x, "═");
    }
    mvprintw(height - 1, width - 1, "╝");

    attroff(COLOR_PAIR(COLOR_BORDERS));
}

int Game::getScore() const
{
    return score;
}

void Game::initColors()
{
    if (has_colors())
    {
        start_color();
        use_default_colors(); // DODAJ TĘ LINIĘ!

        // 3 schematy kolorów
        switch (currentColorScheme)
        {
        case 0: // CLASSIC
            init_pair(COLOR_PLAYER_HEAD, COLOR_CYAN, -1);
            init_pair(COLOR_PLAYER_TRAIL, COLOR_BLUE, -1);
            init_pair(COLOR_PLAYER2_HEAD, COLOR_RED, -1);
            init_pair(COLOR_PLAYER2_TRAIL, COLOR_YELLOW, -1);
            init_pair(COLOR_BORDERS, COLOR_WHITE, -1);
            break;
        case 1: // NEON
            init_pair(COLOR_PLAYER_HEAD, COLOR_MAGENTA, -1);
            init_pair(COLOR_PLAYER_TRAIL, COLOR_YELLOW, -1);
            init_pair(COLOR_PLAYER2_HEAD, COLOR_GREEN, -1);
            init_pair(COLOR_PLAYER2_TRAIL, COLOR_CYAN, -1);
            init_pair(COLOR_BORDERS, COLOR_RED, -1);
            break;
        case 2: // RETRO
            init_pair(COLOR_PLAYER_HEAD, COLOR_GREEN, -1);
            init_pair(COLOR_PLAYER_TRAIL, COLOR_WHITE, -1);
            init_pair(COLOR_PLAYER2_HEAD, COLOR_YELLOW, -1);
            init_pair(COLOR_PLAYER2_TRAIL, COLOR_MAGENTA, -1);
            init_pair(COLOR_BORDERS, COLOR_GREEN, -1);
            break;
        default: // Fallback to CLASSIC
            init_pair(COLOR_PLAYER_HEAD, COLOR_CYAN, -1);
            init_pair(COLOR_PLAYER_TRAIL, COLOR_BLUE, -1);
            init_pair(COLOR_PLAYER2_HEAD, COLOR_RED, -1);
            init_pair(COLOR_PLAYER2_TRAIL, COLOR_YELLOW, -1);
            init_pair(COLOR_BORDERS, COLOR_WHITE, -1);
            break;
        }
        init_pair(COLOR_GAME_OVER, COLOR_RED, -1);    // zamiast COLOR_BLACK
        init_pair(COLOR_HUD, COLOR_YELLOW, -1);       // zamiast COLOR_BLACK
        init_pair(COLOR_MESSAGES, COLOR_MAGENTA, -1); // zamiast COLOR_BLACK
    }
}

void Game::setGameSpeed(GameSpeed speed)
{
    currentGameSpeed = speed;
}

void Game::setColorScheme(int scheme)
{
    currentColorScheme = scheme;
    initColors();
}

std::pair<int, int> Game::getRandomSpawnPosition(int width, int height)
{
    // Initialize random seed
    static bool seeded = false;
    if (!seeded)
    {
        srand(time(nullptr));
        seeded = true;
    }

    // Generate random position with safe margins from borders
    int margin = 3; // Safe distance from walls
    int x = margin + rand() % (width - 2 * margin);
    int y = margin + rand() % (height - 2 * margin);

    return std::make_pair(x, y);
}

void Game::setGameMode(GameMode mode)
{
    currentGameMode = mode;
}

std::pair<std::pair<int, int>, std::pair<int, int>> Game::getTwoPlayerSpawnPositions(int width, int height)
{
    // Initialize random seed if needed
    static bool seeded = false;
    if (!seeded)
    {
        srand(time(nullptr));
        seeded = true;
    }

    int margin = 8;      // Increased margin for safety
    int randomRange = 5; // Small variation range

    // Player 1: Left-top area with small randomization
    int p1_x = margin + rand() % randomRange;
    int p1_y = margin + rand() % randomRange;

    // Player 2: Right-bottom area with small randomization
    int p2_x = width - margin - (rand() % randomRange);
    int p2_y = height - margin - (rand() % randomRange);

    return std::make_pair(
        std::make_pair(p1_x, p1_y), // Player 1 position
        std::make_pair(p2_x, p2_y)  // Player 2 position
    );
}

void Game::handleInputTwoPlayer(Player &player1, Player &player2)
{
    int ch = getch();
    switch (ch)
    {
    // Player 1 controls (arrows)
    case KEY_UP:
        if (state == PLAYING)
            player1.setDirection(UP);
        break;
    case KEY_DOWN:
        if (state == PLAYING)
            player1.setDirection(DOWN);
        break;
    case KEY_LEFT:
        if (state == PLAYING)
            player1.setDirection(LEFT);
        break;
    case KEY_RIGHT:
        if (state == PLAYING)
            player1.setDirection(RIGHT);
        break;

    // Player 2 controls (WASD)
    case 'w':
    case 'W':
        if (state == PLAYING)
            player2.setDirection(UP);
        break;
    case 's':
    case 'S':
        if (state == PLAYING)
            player2.setDirection(DOWN);
        break;
    case 'a':
    case 'A':
        if (state == PLAYING)
            player2.setDirection(LEFT);
        break;
    case 'd':
    case 'D':
        if (state == PLAYING)
            player2.setDirection(RIGHT);
        break;

    // Game controls
    case 'r':
    case 'R':
        if (state == GAME_OVER)
        {
            restartTwoPlayer(player1, player2);
        }
        break;
    case 'q':
    case 'Q':
    case 27:
        stop();
        break;
    }
}

void Game::updateTwoPlayer(Player &player1, Player &player2)
{
    if (state != PLAYING)
        return;

    updateScore();

    // Get next positions for both players
    int p1_nextX = player1.getNextX();
    int p1_nextY = player1.getNextY();
    int p2_nextX = player2.getNextX();
    int p2_nextY = player2.getNextY();

    // Check wall collisions
    bool p1_wallHit = checkWallCollision(p1_nextX, p1_nextY);
    bool p2_wallHit = checkWallCollision(p2_nextX, p2_nextY);

    // Check trail collisions (self)
    bool p1_trailHit = checkTrailCollision(p1_nextX, p1_nextY, player1);
    bool p2_trailHit = checkTrailCollision(p2_nextX, p2_nextY, player2);

    // Check cross-player trail collisions
    bool p1_hitP2Trail = checkTrailCollision(p1_nextX, p1_nextY, player2);
    bool p2_hitP1Trail = checkTrailCollision(p2_nextX, p2_nextY, player1);

    // Check head-to-head collision
    bool headToHead = (p1_nextX == p2_nextX && p1_nextY == p2_nextY);

    // Determine game outcome
    bool p1_loses = p1_wallHit || p1_trailHit || p1_hitP2Trail;
    bool p2_loses = p2_wallHit || p2_trailHit || p2_hitP1Trail;

    if (headToHead)
    {
        // Both players crash into each other - TIE
        gameOverTwoPlayer(0); // 0 = tie
    }
    else if (p1_loses && p2_loses)
    {
        // Both crash simultaneously - TIE
        gameOverTwoPlayer(0); // 0 = tie
    }
    else if (p1_loses)
    {
        // Player 1 loses, Player 2 wins
        gameOverTwoPlayer(2); // 2 = player 2 wins
    }
    else if (p2_loses)
    {
        // Player 2 loses, Player 1 wins
        gameOverTwoPlayer(1); // 1 = player 1 wins
    }
    else
    {
        // No collisions - move both players
        player1.move();
        player2.move();
    }
}

void Game::renderTwoPlayer(Player &player1, Player &player2)
{
    clear();
    drawBorders();

    if (state == PLAYING)
    {
        player1.draw();
        player2.draw();

        // HUD
        attron(COLOR_PAIR(COLOR_HUD));
        mvprintw(0, 3, "╣ Player 1 vs Player 2 ║ Time: %ds ╠", getGameTime());
        int bottomY = height - 1;
        mvprintw(bottomY, 3, "╣ Arrows=P1 ║ WASD=P2 ║ Q=Quit ║ R=Restart ╠");
        attroff(COLOR_PAIR(COLOR_HUD));
    }
    else if (state == GAME_OVER)
    {
        int centerX = width / 2;
        int centerY = height / 2;

        // Winner announcement box
        attron(COLOR_PAIR(COLOR_GAME_OVER));
        mvprintw(centerY - 3, centerX - 15, "╔═══════════════════════════════╗");

        if (winner == 1)
        {
            mvprintw(centerY - 2, centerX - 15, "║        PLAYER 1 WINS!         ║");
        }
        else if (winner == 2)
        {
            mvprintw(centerY - 2, centerX - 15, "║        PLAYER 2 WINS!         ║");
        }
        else
        {
            mvprintw(centerY - 2, centerX - 15, "║           TIE GAME!           ║");
        }

        mvprintw(centerY - 1, centerX - 15, "╠═══════════════════════════════╣");
        mvprintw(centerY, centerX - 15, "║ Time: %2ds   ║  Score: %3d     ║", getGameTime(), score);
        mvprintw(centerY + 1, centerX - 15, "╠═══════════════════════════════╣");
        attroff(COLOR_PAIR(COLOR_GAME_OVER));

        attron(COLOR_PAIR(COLOR_MESSAGES));
        mvprintw(centerY + 2, centerX - 15, "║     R-Restart    Q-Quit       ║");
        mvprintw(centerY + 3, centerX - 15, "╚═══════════════════════════════╝");
        attroff(COLOR_PAIR(COLOR_MESSAGES));
    }

    refresh();
}

void Game::gameOverTwoPlayer(int winnerPlayer)
{
    winner = winnerPlayer;
    state = GAME_OVER;
}

void Game::restartTwoPlayer(Player &player1, Player &player2)
{
    // Generate new spawn positions
    int actualWidth, actualHeight;
    getmaxyx(stdscr, actualHeight, actualWidth);
    auto spawnPositions = getTwoPlayerSpawnPositions(actualWidth, actualHeight);

    // Reset game state
    startGame();

    // Reset both players with new positions
    player1.reset(spawnPositions.first.first, spawnPositions.first.second);
    player2.reset(spawnPositions.second.first, spawnPositions.second.second);

    winner = 0; // Reset winner
}