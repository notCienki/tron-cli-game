#include "../include/player.h"
#include <ncurses.h>

char TrailSegment::getChar() const
{
    if (from == to)
    {
        switch (from)
        {
        case UP:
        case DOWN:
            return '|'; // Zwracamy ASCII char dla identyfikacji
        case LEFT:
        case RIGHT:
            return '-';
        default:
            return '-';
        }
    }
    else
    {
        return '+'; // ASCII char dla zakrętów
    }
}

const char *TrailSegment::getUnicodeChar() const
{
    // Jeśli to głowa gracza, zwróć symbol gracza
    if (isHead)
    {
        switch (to)
        { // używamy 'to' jako aktualny kierunek
        case UP:
            return "⇡";
        case DOWN:
            return "⇣";
        case LEFT:
            return "⇠";
        case RIGHT:
            return "⇢";
        default:
            return "*";
        }
    }

    if (from == to)
    {
        switch (from)
        {
        case UP:
        case DOWN:
            return "┆"; // Unicode vertical
        case LEFT:
        case RIGHT:
            return "┄"; // Unicode horizontal
        default:
            return "┄"; // Default horizontal
        }
    }
    else
    {
        if ((from == LEFT && to == DOWN) || (from == UP && to == RIGHT))
            return "┌"; // ┌ lewy górny róg
        if ((from == RIGHT && to == DOWN) || (from == UP && to == LEFT))
            return "┐"; // ┐ prawy górny róg
        if ((from == LEFT && to == UP) || (from == DOWN && to == RIGHT))
            return "└"; // └ lewy dolny róg
        if ((from == RIGHT && to == UP) || (from == DOWN && to == LEFT))
            return "┘"; // ┘ prawy dolny róg
    }

    return "┄";
}

Player::Player(int startX, int startY)
    : x(startX), y(startY), startX(startX), startY(startY),
      direction(RIGHT), lastDirection(RIGHT)
{
    trail.push_back(TrailSegment(x, y, direction, direction, true)); // Head segment
}

void Player::move()
{
    // Przekonwertuj obecną głowę na segment trail i ustaw zakręt
    if (!trail.empty())
    {
        trail.back().to = direction; // To tworzy zakręt!
        trail.back().isHead = false;
    }

    // Zaktualizuj pozycję gracza
    switch (direction)
    {
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    }

    // Dodaj nową głowę (zawsze prosta linia)
    trail.push_back(TrailSegment(x, y, direction, direction, true));

    // Zaktualizuj lastDirection
    lastDirection = direction;
}

void Player::setDirection(Direction newDir)
{
    // Prevent reversing direction
    if ((direction == UP && newDir == DOWN) ||
        (direction == DOWN && newDir == UP) ||
        (direction == LEFT && newDir == RIGHT) ||
        (direction == RIGHT && newDir == LEFT))
    {
        return; // Don't allow reversing
    }

    direction = newDir; // Update direction
}

char Player::getPlayerChar() const
{
    switch (direction)
    {
    case UP:
        return '^';
    case DOWN:
        return 'v';
    case LEFT:
        return '<';
    case RIGHT:
        return '>';
    }
    return '*';
}

const char *Player::getPlayerUnicodeChar() const
{
    switch (direction)
    {
    case UP:
        return "⇡";
    case DOWN:
        return "⇣";
    case LEFT:
        return "⇠";
    case RIGHT:
        return "⇢";
    }
    return "*";
}

void Player::draw()
{
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    // Rysuj wszystkie segmenty (łącznie z głową)
    for (size_t i = 0; i < trail.size(); i++)
    {
        const auto &segment = trail[i];
        if (segment.isHead)
        {
            attron(COLOR_PAIR(1)); // CYAN dla głowy
        }
        else
        {
            attron(COLOR_PAIR(2)); // GREEN dla śladu
        }
        if (segment.y >= 0 && segment.y < maxY &&
            segment.x >= 0 && segment.x < maxX)
        {
            mvprintw(segment.y, segment.x, "%s", segment.getUnicodeChar());
            attroff(COLOR_PAIR(segment.isHead ? 1 : 2));
        }
    }
}

int Player::getNextX() const
{
    switch (direction)
    {
    case LEFT:
        return x - 1;
    case RIGHT:
        return x + 1;
    default:
        return x;
    }
}

int Player::getNextY() const
{
    switch (direction)
    {
    case UP:
        return y - 1;
    case DOWN:
        return y + 1;
    default:
        return y;
    }
}

void Player::reset()
{
    x = startX; // Reset to starting position
    y = startY;
    direction = RIGHT;                                               // Reset direction to default
    trail.clear();                                                   // Clear the trial history
    trail.push_back(TrailSegment(x, y, direction, direction, true)); // Add starting head segment
}