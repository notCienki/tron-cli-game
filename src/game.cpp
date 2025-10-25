#include "../include/game.h"
#include "../include/player.h"
#include <unistd.h>
#include <cstdio>
#include <random>
#include <ctime>

Game::Game(int w, int h) : width(w), height(h), running(false), state(PLAYING), currentGameSpeed(NORMAL), currentGameMode(SINGLE_PLAYER), currentColorScheme(0), firstStart(true), gameBot(nullptr), score(0), winner(Config::WINNER_TIE) {}

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

    mvprintw(centerY - Config::WELCOME_BOX_VERTICAL_OFFSET, centerX - Config::MENU_BOX_HALF_WIDTH, "╔══════════════════════╗");
    mvprintw(centerY - 2, centerX - Config::MENU_BOX_HALF_WIDTH, "║      TRON GAME       ║");
    mvprintw(centerY - 1, centerX - Config::MENU_BOX_HALF_WIDTH, "╠══════════════════════╣");
    mvprintw(centerY, centerX - Config::MENU_BOX_HALF_WIDTH, "║   Use ⇠⇡⇢⇣ to move   ║");
    mvprintw(centerY + 1, centerX - Config::MENU_BOX_HALF_WIDTH, "║ Avoid walls & trails ║");
    mvprintw(centerY + 2, centerX - Config::MENU_BOX_HALF_WIDTH, "╚══════════════════════╝");

    refresh();
    sleep(Config::WELCOME_MESSAGE_DELAY_SEC);
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

    width = actualWidth;
    height = actualHeight;

    if (currentGameMode == SINGLE_PLAYER)
    {
        int randomSide = rand() % Config::NUM_SIDES;
        auto spawnPos = getRandomPositionOnSide(randomSide, actualWidth, actualHeight);
        Direction safeDir = getSafeDirection(randomSide);
        Player player(spawnPos.first, spawnPos.second, Config::PLAYER_1_ID, safeDir);
        player.initializeTrail();

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
        int side1 = rand() % Config::NUM_SIDES;
        int side2 = (side1 + 2) % Config::NUM_SIDES;

        auto p1_pos = getRandomPositionOnSide(side1, actualWidth, actualHeight);
        auto p2_pos = getRandomPositionOnSide(side2, actualWidth, actualHeight);

        Direction safeDir1 = getSafeDirection(side1);
        Direction safeDir2 = getSafeDirection(side2);
        Player player1(p1_pos.first, p1_pos.second, Config::PLAYER_1_ID, safeDir1);
        Player player2(p2_pos.first, p2_pos.second, Config::PLAYER_2_ID, safeDir2);

        player1.initializeTrail();
        player2.initializeTrail();

        while (running)
        {
            handleInputTwoPlayer(player1, player2);
            updateTwoPlayer(player1, player2);
            renderTwoPlayer(player1, player2);
            usleep(currentGameSpeed);
        }
    }
    else if (currentGameMode == VS_BOT)
    {
        int side1 = rand() % Config::NUM_SIDES;
        int side2 = (side1 + 2) % Config::NUM_SIDES;
        if (!gameBot)
        {
            auto bot_pos = getRandomPositionOnSide(side2, actualWidth, actualHeight);
            Direction safeDir2 = getSafeDirection(side2);
            gameBot = new Bot(bot_pos.first, bot_pos.second, Config::DEFAULT_BOT_DIFFICULTY, safeDir2);
            gameBot->getPlayer()->initializeTrail();
        }

        auto p1_pos = getRandomPositionOnSide(side1, actualWidth, actualHeight);
        Direction safeDir1 = getSafeDirection(side1);
        Player player(p1_pos.first, p1_pos.second, Config::PLAYER_1_ID, safeDir1);
        player.initializeTrail();

        while (running)
        {
            handleInputVsBot(player, *gameBot);
            updateVsBot(player, *gameBot);
            renderVsBot(player, *gameBot);
            usleep(currentGameSpeed);
        }
    }
}

void Game::handleInput(Player &player)
{
    int ch = getch();
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
    case 27:
        stop();
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

    drawBorders();
    if (state == PLAYING)
    {
        player.draw();

        attron(COLOR_PAIR(Config::COLOR_HUD));
        mvprintw(0, Config::HUD_HORIZONTAL_OFFSET, "╣ Score: %d ║ Time: %ds ╠", score, getGameTime());
        int bottomY = height - 1;
        mvprintw(bottomY, Config::HUD_HORIZONTAL_OFFSET, "╣ ⇠⇡⇢⇣ Move ║ Q Quit ║ R Restart ╠");
        attroff(COLOR_PAIR(Config::COLOR_HUD));
    }
    else if (state == GAME_OVER)
    {
        int centerX = width / 2;
        int centerY = height / 2;

        attron(COLOR_PAIR(Config::COLOR_GAME_OVER));
        mvprintw(centerY - Config::GAMEOVER_BOX_VERTICAL_OFFSET, centerX - Config::MENU_BOX_HALF_WIDTH, "╔══════════════════════╗");
        mvprintw(centerY - 2, centerX - Config::MENU_BOX_HALF_WIDTH, "║      GAME OVER!      ║");
        mvprintw(centerY - 1, centerX - Config::MENU_BOX_HALF_WIDTH, "╠══════════════════════╣");
        mvprintw(centerY, centerX - Config::MENU_BOX_HALF_WIDTH, "║ Score:%3d   Time:%2ds ║", score, getGameTime());
        mvprintw(centerY + 1, centerX - Config::MENU_BOX_HALF_WIDTH, "╠══════════════════════╣");
        attroff(COLOR_PAIR(Config::COLOR_GAME_OVER));

        attron(COLOR_PAIR(Config::COLOR_MESSAGES));
        mvprintw(centerY + 2, centerX - Config::MENU_BOX_HALF_WIDTH, "║   R-Restart  Q-Quit  ║");
        mvprintw(centerY + 3, centerX - Config::MENU_BOX_HALF_WIDTH, "╚══════════════════════╝");
        attroff(COLOR_PAIR(Config::COLOR_MESSAGES));
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
    return (x <= 0 || x >= width - 1 || y <= 0 || y >= height - 1);
}

void Game::gameOver()
{
    state = GAME_OVER;
}

void Game::restart(Player &player)
{
    int actualWidth, actualHeight;
    getmaxyx(stdscr, actualHeight, actualWidth);

    int randomSide = rand() % Config::NUM_SIDES;
    auto spawnPos = getRandomPositionOnSide(randomSide, actualWidth, actualHeight);
    Direction safeDir = getSafeDirection(randomSide);

    startGame();

    player.reset(spawnPos.first, spawnPos.second);
    player.setDirection(safeDir);
}

bool Game::checkTrailCollision(int x, int y, const Player &player)
{
    const auto &trail = player.getTrail();

    if (trail.size() <= Config::COLLISION_TRAIL_MIN_LENGTH)
    {
        return false;
    }

    for (size_t i = 0; i < trail.size() - Config::COLLISION_TRAIL_MIN_LENGTH; i++)
    {
        if (trail[i].x == x && trail[i].y == y)
        {
            return true;
        }
    }

    return false;
}

void Game::drawBorders()
{
    attron(COLOR_PAIR(Config::COLOR_BORDERS));

    mvprintw(0, 0, "╔");
    for (int x = 1; x < width - 1; x++)
    {
        mvprintw(0, x, "═");
    }
    mvprintw(0, width - 1, "╗");

    for (int y = 1; y < height - 1; y++)
    {
        mvprintw(y, 0, "║");
        mvprintw(y, width - 1, "║");
    }

    mvprintw(height - 1, 0, "╚");
    for (int x = 1; x < width - 1; x++)
    {
        mvprintw(height - 1, x, "═");
    }
    mvprintw(height - 1, width - 1, "╝");

    attroff(COLOR_PAIR(Config::COLOR_BORDERS));
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
        use_default_colors();

        switch (currentColorScheme)
        {
        case 0:
            init_pair(Config::COLOR_PLAYER_HEAD, COLOR_CYAN, -1);
            init_pair(Config::COLOR_PLAYER_TRAIL, COLOR_BLUE, -1);
            init_pair(Config::COLOR_PLAYER2_HEAD, COLOR_RED, -1);
            init_pair(Config::COLOR_PLAYER2_TRAIL, COLOR_YELLOW, -1);
            init_pair(Config::COLOR_BORDERS, COLOR_WHITE, -1);
            break;
        case 1:
            init_pair(Config::COLOR_PLAYER_HEAD, COLOR_MAGENTA, -1);
            init_pair(Config::COLOR_PLAYER_TRAIL, COLOR_YELLOW, -1);
            init_pair(Config::COLOR_PLAYER2_HEAD, COLOR_GREEN, -1);
            init_pair(Config::COLOR_PLAYER2_TRAIL, COLOR_CYAN, -1);
            init_pair(Config::COLOR_BORDERS, COLOR_RED, -1);
            break;
        case 2:
            init_pair(Config::COLOR_PLAYER_HEAD, COLOR_GREEN, -1);
            init_pair(Config::COLOR_PLAYER_TRAIL, COLOR_WHITE, -1);
            init_pair(Config::COLOR_PLAYER2_HEAD, COLOR_YELLOW, -1);
            init_pair(Config::COLOR_PLAYER2_TRAIL, COLOR_MAGENTA, -1);
            init_pair(Config::COLOR_BORDERS, COLOR_GREEN, -1);
            break;
        default:
            init_pair(Config::COLOR_PLAYER_HEAD, COLOR_CYAN, -1);
            init_pair(Config::COLOR_PLAYER_TRAIL, COLOR_BLUE, -1);
            init_pair(Config::COLOR_PLAYER2_HEAD, COLOR_RED, -1);
            init_pair(Config::COLOR_PLAYER2_TRAIL, COLOR_YELLOW, -1);
            init_pair(Config::COLOR_BORDERS, COLOR_WHITE, -1);
            break;
        }
        init_pair(Config::COLOR_GAME_OVER, COLOR_RED, -1);
        init_pair(Config::COLOR_HUD, COLOR_YELLOW, -1);
        init_pair(Config::COLOR_MESSAGES, COLOR_MAGENTA, -1);
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
    static bool seeded = false;
    if (!seeded)
    {
        srand(time(nullptr));
        seeded = true;
    }

    int x = Config::SPAWN_MARGIN + rand() % (width - 2 * Config::SPAWN_MARGIN);
    int y = Config::SPAWN_MARGIN + rand() % (height - 2 * Config::SPAWN_MARGIN);

    return std::make_pair(x, y);
}

void Game::setGameMode(GameMode mode)
{
    currentGameMode = mode;
}

std::pair<int, int> Game::getRandomPositionOnSide(int side, int width, int height)
{
    int x, y;

    switch (side)
    {
    case Config::SIDE_TOP:
        x = Config::SPAWN_MARGIN + rand() % (width - 2 * Config::SPAWN_MARGIN);
        y = Config::SPAWN_MARGIN;
        break;
    case Config::SIDE_RIGHT:
        x = width - Config::SPAWN_MARGIN;
        y = Config::SPAWN_MARGIN + rand() % (height - 2 * Config::SPAWN_MARGIN);
        break;
    case Config::SIDE_BOTTOM:
        x = Config::SPAWN_MARGIN + rand() % (width - 2 * Config::SPAWN_MARGIN);
        y = height - Config::SPAWN_MARGIN;
        break;
    case Config::SIDE_LEFT:
        x = Config::SPAWN_MARGIN;
        y = Config::SPAWN_MARGIN + rand() % (height - 2 * Config::SPAWN_MARGIN);
        break;
    }

    return std::make_pair(x, y);
}

Direction Game::getSafeDirection(int side)
{
    switch (side)
    {
    case Config::SIDE_TOP:
        return DOWN;
    case Config::SIDE_RIGHT:
        return LEFT;
    case Config::SIDE_BOTTOM:
        return UP;
    case Config::SIDE_LEFT:
        return RIGHT;
    default:
        return RIGHT;
    }
}

std::pair<std::pair<int, int>, std::pair<int, int>> Game::getTwoPlayerSpawnPositions(int width, int height)
{
    static bool seeded = false;
    if (!seeded)
    {
        srand(time(nullptr));
        seeded = true;
    }

    int side1 = rand() % Config::NUM_SIDES;
    int side2 = (side1 + 2) % Config::NUM_SIDES;

    auto p1_pos = getRandomPositionOnSide(side1, width, height);
    auto p2_pos = getRandomPositionOnSide(side2, width, height);

    return std::make_pair(p1_pos, p2_pos);
}

void Game::handleInputTwoPlayer(Player &player1, Player &player2)
{
    int ch = getch();
    switch (ch)
    {
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

    int p1_nextX = player1.getNextX();
    int p1_nextY = player1.getNextY();
    int p2_nextX = player2.getNextX();
    int p2_nextY = player2.getNextY();

    bool p1_wallHit = checkWallCollision(p1_nextX, p1_nextY);
    bool p2_wallHit = checkWallCollision(p2_nextX, p2_nextY);

    bool p1_trailHit = checkTrailCollision(p1_nextX, p1_nextY, player1);
    bool p2_trailHit = checkTrailCollision(p2_nextX, p2_nextY, player2);

    bool p1_hitP2Trail = checkTrailCollision(p1_nextX, p1_nextY, player2);
    bool p2_hitP1Trail = checkTrailCollision(p2_nextX, p2_nextY, player1);

    bool headToHead = (p1_nextX == p2_nextX && p1_nextY == p2_nextY);

    bool p1_loses = p1_wallHit || p1_trailHit || p1_hitP2Trail;
    bool p2_loses = p2_wallHit || p2_trailHit || p2_hitP1Trail;

    if (headToHead)
    {
        gameOverTwoPlayer(Config::WINNER_TIE);
    }
    else if (p1_loses && p2_loses)
    {
        gameOverTwoPlayer(Config::WINNER_TIE);
    }
    else if (p1_loses)
    {
        gameOverTwoPlayer(Config::WINNER_PLAYER2);
    }
    else if (p2_loses)
    {
        gameOverTwoPlayer(Config::WINNER_PLAYER1);
    }
    else
    {
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

        attron(COLOR_PAIR(Config::COLOR_HUD));
        mvprintw(0, Config::HUD_HORIZONTAL_OFFSET, "╣ Player 1 vs Player 2 ║ Time: %ds ╠", getGameTime());
        int bottomY = height - 1;
        mvprintw(bottomY, Config::HUD_HORIZONTAL_OFFSET, "╣ Arrows=P1 ║ WASD=P2 ║ Q=Quit ║ R=Restart ╠");
        attroff(COLOR_PAIR(Config::COLOR_HUD));
    }
    else if (state == GAME_OVER)
    {
        int centerX = width / 2;
        int centerY = height / 2;

        attron(COLOR_PAIR(Config::COLOR_GAME_OVER));
        mvprintw(centerY - Config::GAMEOVER_BOX_VERTICAL_OFFSET, centerX - Config::MENU_BOX_LARGE_HALF_WIDTH, "╔═══════════════════════════════╗");

        if (winner == Config::WINNER_PLAYER1)
        {
            mvprintw(centerY - 2, centerX - Config::MENU_BOX_LARGE_HALF_WIDTH, "║        PLAYER 1 WINS!         ║");
        }
        else if (winner == Config::WINNER_PLAYER2)
        {
            mvprintw(centerY - 2, centerX - Config::MENU_BOX_LARGE_HALF_WIDTH, "║        PLAYER 2 WINS!         ║");
        }
        else
        {
            mvprintw(centerY - 2, centerX - Config::MENU_BOX_LARGE_HALF_WIDTH, "║           TIE GAME!           ║");
        }

        mvprintw(centerY - 1, centerX - Config::MENU_BOX_LARGE_HALF_WIDTH, "╠═══════════════════════════════╣");
        mvprintw(centerY, centerX - Config::MENU_BOX_LARGE_HALF_WIDTH, "║ Time: %2ds   ║  Score: %3d     ║", getGameTime(), score);
        mvprintw(centerY + 1, centerX - Config::MENU_BOX_LARGE_HALF_WIDTH, "╠═══════════════════════════════╣");
        attroff(COLOR_PAIR(Config::COLOR_GAME_OVER));

        attron(COLOR_PAIR(Config::COLOR_MESSAGES));
        mvprintw(centerY + 2, centerX - Config::MENU_BOX_LARGE_HALF_WIDTH, "║     R-Restart    Q-Quit       ║");
        mvprintw(centerY + 3, centerX - Config::MENU_BOX_LARGE_HALF_WIDTH, "╚═══════════════════════════════╝");
        attroff(COLOR_PAIR(Config::COLOR_MESSAGES));
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
    int actualWidth, actualHeight;
    getmaxyx(stdscr, actualHeight, actualWidth);

    int side1 = rand() % Config::NUM_SIDES;
    int side2 = (side1 + 2) % Config::NUM_SIDES;

    auto p1_pos = getRandomPositionOnSide(side1, actualWidth, actualHeight);
    auto p2_pos = getRandomPositionOnSide(side2, actualWidth, actualHeight);

    Direction safeDir1 = getSafeDirection(side1);
    Direction safeDir2 = getSafeDirection(side2);

    startGame();

    player1.reset(p1_pos.first, p1_pos.second);
    player1.setDirection(safeDir1);
    player2.reset(p2_pos.first, p2_pos.second);
    player2.setDirection(safeDir2);

    winner = Config::WINNER_TIE;
}

void Game::handleInputVsBot(Player &player, Bot &bot)
{
    int ch = getch();
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
            restartVsBot(player, bot);
        }
        break;
    case 'q':
    case 'Q':
    case 27:
        stop();
        break;
    }
}

void Game::updateVsBot(Player &player, Bot &bot)
{
    if (state != PLAYING)
        return;

    updateScore();

    bot.update(player, width, height);

    int p_nextX = player.getNextX();
    int p_nextY = player.getNextY();
    int b_nextX = bot.getPlayer()->getNextX();
    int b_nextY = bot.getPlayer()->getNextY();

    bool p_wallHit = checkWallCollision(p_nextX, p_nextY);
    bool b_wallHit = checkWallCollision(b_nextX, b_nextY);

    bool p_trailHit = checkTrailCollision(p_nextX, p_nextY, player);
    bool b_trailHit = checkTrailCollision(b_nextX, b_nextY, *bot.getPlayer());

    bool p_hitBotTrail = checkTrailCollision(p_nextX, p_nextY, *bot.getPlayer());
    bool b_hitPlayerTrail = checkTrailCollision(b_nextX, b_nextY, player);

    bool headToHead = (p_nextX == b_nextX && p_nextY == b_nextY);

    bool p_loses = p_wallHit || p_trailHit || p_hitBotTrail;
    bool b_loses = b_wallHit || b_trailHit || b_hitPlayerTrail;

    if (headToHead || (p_loses && b_loses))
    {
        gameOverTwoPlayer(Config::WINNER_TIE);
    }
    else if (p_loses)
    {
        gameOverTwoPlayer(Config::WINNER_PLAYER2);
    }
    else if (b_loses)
    {
        gameOverTwoPlayer(Config::WINNER_PLAYER1);
    }
    else
    {
        player.move();
        bot.getPlayer()->move();
    }
}

void Game::renderVsBot(Player &player, Bot &bot)
{
    clear();
    drawBorders();

    if (state == PLAYING)
    {
        player.draw();
        bot.getPlayer()->draw();

        attron(COLOR_PAIR(Config::COLOR_HUD));
        mvprintw(0, Config::HUD_HORIZONTAL_OFFSET, "╣ Player vs Bot ║ Time: %ds ╠", getGameTime());
        int bottomY = height - 1;
        mvprintw(bottomY, Config::HUD_HORIZONTAL_OFFSET, "╣ Arrows=Move ║ Q=Quit ║ R=Restart ╠");
        attroff(COLOR_PAIR(Config::COLOR_HUD));
    }
    else if (state == GAME_OVER)
    {
        int centerX = width / 2;
        int centerY = height / 2;

        attron(COLOR_PAIR(Config::COLOR_GAME_OVER));
        mvprintw(centerY - Config::GAMEOVER_BOX_VERTICAL_OFFSET, centerX - Config::MENU_BOX_LARGE_HALF_WIDTH, "╔═══════════════════════════════╗");

        if (winner == Config::WINNER_PLAYER1)
        {
            mvprintw(centerY - 2, centerX - Config::MENU_BOX_LARGE_HALF_WIDTH, "║        PLAYER WINS!           ║");
        }
        else if (winner == Config::WINNER_PLAYER2)
        {
            mvprintw(centerY - 2, centerX - Config::MENU_BOX_LARGE_HALF_WIDTH, "║         BOT WINS!             ║");
        }
        else
        {
            mvprintw(centerY - 2, centerX - Config::MENU_BOX_LARGE_HALF_WIDTH, "║         TIE GAME!             ║");
        }

        mvprintw(centerY - 1, centerX - Config::MENU_BOX_LARGE_HALF_WIDTH, "╠═══════════════════════════════╣");
        mvprintw(centerY, centerX - Config::MENU_BOX_LARGE_HALF_WIDTH, "║ Time: %2ds   ║  Score: %3d     ║", getGameTime(), score);
        mvprintw(centerY + 1, centerX - Config::MENU_BOX_LARGE_HALF_WIDTH, "╠═══════════════════════════════╣");
        attroff(COLOR_PAIR(Config::COLOR_GAME_OVER));

        attron(COLOR_PAIR(Config::COLOR_MESSAGES));
        mvprintw(centerY + 2, centerX - Config::MENU_BOX_LARGE_HALF_WIDTH, "║     R-Restart    Q-Quit       ║");
        mvprintw(centerY + 3, centerX - Config::MENU_BOX_LARGE_HALF_WIDTH, "╚═══════════════════════════════╝");
        attroff(COLOR_PAIR(Config::COLOR_MESSAGES));
    }

    refresh();
}

void Game::restartVsBot(Player &player, Bot &bot)
{
    int actualWidth, actualHeight;
    getmaxyx(stdscr, actualHeight, actualWidth);

    int side1 = rand() % Config::NUM_SIDES;
    int side2 = (side1 + 2) % Config::NUM_SIDES;

    auto p_pos = getRandomPositionOnSide(side1, actualWidth, actualHeight);
    auto b_pos = getRandomPositionOnSide(side2, actualWidth, actualHeight);

    Direction safeDir1 = getSafeDirection(side1);
    Direction safeDir2 = getSafeDirection(side2);

    startGame();

    player.reset(p_pos.first, p_pos.second);
    player.setDirection(safeDir1);
    bot.reset(b_pos.first, b_pos.second);
    bot.getPlayer()->setDirection(safeDir2);

    winner = Config::WINNER_TIE;
}