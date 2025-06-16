#include "../include/Game.h"
#include "../include/entities/player.h"
#include "../include/core/GameConfig.h"
#include <unistd.h>
#include <cstdio>
#include <random>
#include <ctime>

Game::Game(int w, int h) : width(w), height(h), running(false), state(PLAYING),
                           currentGameSpeed(NORMAL), currentGameMode(SINGLE_PLAYER), currentColorScheme(0),
                           firstStart(true), gameBot(nullptr), score(0), winner(0),
                           inputManager(std::make_unique<InputManager>()),
                           gameConfig(std::make_shared<GameConfig>()),
                           renderer(std::make_shared<Renderer>(gameConfig))
{
}

Game::~Game()
{
    cleanup();
}

void Game::init()
{

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
        renderer->clear();
        renderer->drawBorders();
        renderer->drawWelcomeMessage();
        renderer->refresh();
        sleep(GameConfig::WELCOME_MESSAGE_DURATION_SEC);
        firstStart = false;
    }
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
        int randomSide = rand() % GameConfig::SPAWN_SIDES_COUNT;
        auto spawnPos = getRandomPositionOnSide(randomSide, actualWidth, actualHeight);
        Direction safeDir = getSafeDirection(randomSide);
        Player player(spawnPos.first, spawnPos.second, 1, safeDir);
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
        int side1 = rand() % GameConfig::SPAWN_SIDES_COUNT;
        int side2 = (side1 + 2) % 4;

        auto p1_pos = getRandomPositionOnSide(side1, actualWidth, actualHeight);
        auto p2_pos = getRandomPositionOnSide(side2, actualWidth, actualHeight);

        Direction safeDir1 = getSafeDirection(side1);
        Direction safeDir2 = getSafeDirection(side2);
        Player player1(p1_pos.first, p1_pos.second, 1, safeDir1);
        Player player2(p2_pos.first, p2_pos.second, 2, safeDir2);

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
        int side1 = rand() % GameConfig::SPAWN_SIDES_COUNT;
        int side2 = (side1 + 2) % 4;
        if (!gameBot)
        {
            auto bot_pos = getRandomPositionOnSide(side2, actualWidth, actualHeight);
            Direction safeDir2 = getSafeDirection(side2);
            gameBot = new Bot(bot_pos.first, bot_pos.second, 1, safeDir2);
            gameBot->getPlayer()->initializeTrail();
        }

        auto p1_pos = getRandomPositionOnSide(side1, actualWidth, actualHeight);
        Direction safeDir1 = getSafeDirection(side1);
        Player player(p1_pos.first, p1_pos.second, 1, safeDir1);
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
    inputManager->pollInput();

    while (inputManager->hasInput())
    {
        int key = inputManager->getNextInput();

        if (InputManager::isMovementKey(key) && state == PLAYING)
        {
            if (InputManager::isPlayer1Key(key))
            {
                player.setDirection(inputManager->keyToDirection(key));
            }
        }
        else if (InputManager::isActionKey(key))
        {
            if (key == 'r' || key == 'R')
            {
                restart(player);
            }
            else if (key == 'q' || key == 'Q' || key == 27) // 27 is ESC key
            {
                stop();
            }
        }
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
    std::vector<Player *> players = {&player};
    renderer->renderGameScreen(state, players, "Single Player", score, getGameTime(), winner);
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

    int randomSide = rand() % GameConfig::SPAWN_SIDES_COUNT;
    auto spawnPos = getRandomPositionOnSide(randomSide, actualWidth, actualHeight);
    Direction safeDir = getSafeDirection(randomSide);

    startGame();

    player.reset(spawnPos.first, spawnPos.second);
    player.setDirection(safeDir);
}

bool Game::checkTrailCollision(int x, int y, const Player &player)
{
    const auto &trail = player.getTrail();

    if (trail.size() <= GameConfig::MIN_TRAIL_LENGTH_FOR_COLLISION)
    {
        return false;
    }

    for (size_t i = 0; i < trail.size() - 2; i++)
    {
        if (trail[i].x == x && trail[i].y == y)
        {
            return true;
        }
    }

    return false;
}

int Game::getScore() const
{
    return score;
}

void Game::setGameSpeed(GameSpeed speed)
{
    currentGameSpeed = speed;
}

std::pair<int, int> Game::getRandomSpawnPosition(int width, int height)
{
    static bool seeded = false;
    if (!seeded)
    {
        srand(time(nullptr));
        seeded = true;
    }

    int margin = GameConfig::SPAWN_MARGIN;
    int x = margin + rand() % (width - 2 * margin);
    int y = margin + rand() % (height - 2 * margin);

    return std::make_pair(x, y);
}

void Game::setGameMode(GameMode mode)
{
    currentGameMode = mode;
}

std::pair<int, int> Game::getRandomPositionOnSide(int side, int width, int height)
{
    int margin = GameConfig::SPAWN_MARGIN;
    int x, y;

    switch (side)
    {
    case 0:
        x = margin + rand() % (width - 2 * margin);
        y = margin;
        break;
    case 1:
        x = width - margin;
        y = margin + rand() % (height - 2 * margin);
        break;
    case 2:
        x = margin + rand() % (width - 2 * margin);
        y = height - margin;
        break;
    case 3:
        x = margin;
        y = margin + rand() % (height - 2 * margin);
        break;
    }

    return std::make_pair(x, y);
}

Direction Game::getSafeDirection(int side)
{
    switch (side)
    {
    case 0:
        return DOWN;
    case 1:
        return LEFT;
    case 2:
        return UP;
    case 3:
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

    int side1 = rand() % GameConfig::SPAWN_SIDES_COUNT;
    int side2 = (side1 + 2) % 4;

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
        gameOverTwoPlayer(0);
    }
    else if (p1_loses && p2_loses)
    {
        gameOverTwoPlayer(0);
    }
    else if (p1_loses)
    {
        gameOverTwoPlayer(2);
    }
    else if (p2_loses)
    {
        gameOverTwoPlayer(1);
    }
    else
    {
        player1.move();
        player2.move();
    }
}

void Game::renderTwoPlayer(Player &player1, Player &player2)
{
    std::vector<Player *> players = {&player1, &player2};
    renderer->renderGameScreen(state, players, "Two Player", score, getGameTime(), winner);
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

    int side1 = rand() % GameConfig::SPAWN_SIDES_COUNT;
    int side2 = (side1 + 2) % 4;

    auto p1_pos = getRandomPositionOnSide(side1, actualWidth, actualHeight);
    auto p2_pos = getRandomPositionOnSide(side2, actualWidth, actualHeight);

    Direction safeDir1 = getSafeDirection(side1);
    Direction safeDir2 = getSafeDirection(side2);

    startGame();

    player1.reset(p1_pos.first, p1_pos.second);
    player1.setDirection(safeDir1);
    player2.reset(p2_pos.first, p2_pos.second);
    player2.setDirection(safeDir2);

    winner = 0;
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
        gameOverTwoPlayer(0);
    }
    else if (p_loses)
    {
        gameOverTwoPlayer(2);
    }
    else if (b_loses)
    {
        gameOverTwoPlayer(1);
    }
    else
    {
        player.move();
        bot.getPlayer()->move();
    }
}

void Game::renderVsBot(Player &player, Bot &bot)
{
    std::vector<Player *> players = {&player, bot.getPlayer()};
    renderer->renderGameScreen(state, players, "vs Bot", score, getGameTime(), winner);
}

void Game::restartVsBot(Player &player, Bot &bot)
{
    int actualWidth, actualHeight;
    getmaxyx(stdscr, actualHeight, actualWidth);

    int side1 = rand() % GameConfig::SPAWN_SIDES_COUNT;
    int side2 = (side1 + 2) % 4;

    auto p_pos = getRandomPositionOnSide(side1, actualWidth, actualHeight);
    auto b_pos = getRandomPositionOnSide(side2, actualWidth, actualHeight);

    Direction safeDir1 = getSafeDirection(side1);
    Direction safeDir2 = getSafeDirection(side2);

    startGame();

    player.reset(p_pos.first, p_pos.second);
    player.setDirection(safeDir1);
    bot.reset(b_pos.first, b_pos.second);
    bot.getPlayer()->setDirection(safeDir2);

    winner = 0;
}