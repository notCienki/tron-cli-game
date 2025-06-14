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
            return '|';
        case LEFT:
        case RIGHT:
            return '-';
        default:
            return '-';
        }
    }
    else
    {
        return '+';
    }
}

const char *TrailSegment::getUnicodeChar() const
{
    if (isHead)
    {
        switch (to)
        {
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
            return "┆";
        case LEFT:
        case RIGHT:
            return "┄";
        default:
            return "┄";
        }
    }
    else
    {
        if ((from == LEFT && to == DOWN) || (from == UP && to == RIGHT))
            return "┌";
        if ((from == RIGHT && to == DOWN) || (from == UP && to == LEFT))
            return "┐";
        if ((from == LEFT && to == UP) || (from == DOWN && to == RIGHT))
            return "└";
        if ((from == RIGHT && to == UP) || (from == DOWN && to == LEFT))
            return "┘";
    }

    return "┄";
}

Player::Player(int startX, int startY, int id)
    : x(startX), y(startY), startX(startX), startY(startY),
      direction(RIGHT), lastDirection(RIGHT), playerId(id)
{
    trail.push_back(TrailSegment(x, y, direction, direction, true));
}

void Player::move()
{
    if (!trail.empty())
    {
        trail.back().to = direction;
        trail.back().isHead = false;
    }

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

    trail.push_back(TrailSegment(x, y, direction, direction, true));

    lastDirection = direction;
}

void Player::setDirection(Direction newDir)
{
    if ((direction == UP && newDir == DOWN) ||
        (direction == DOWN && newDir == UP) ||
        (direction == LEFT && newDir == RIGHT) ||
        (direction == RIGHT && newDir == LEFT))
    {
        return;
    }

    direction = newDir;
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

    for (size_t i = 0; i < trail.size(); i++)
    {
        const auto &segment = trail[i];

        int headColor = (playerId == 1) ? COLOR_PLAYER_HEAD : COLOR_PLAYER2_HEAD;
        int trailColor = (playerId == 1) ? COLOR_PLAYER_TRAIL : COLOR_PLAYER2_TRAIL;

        if (segment.isHead)
        {
            attron(COLOR_PAIR(headColor));
        }
        else
        {
            attron(COLOR_PAIR(trailColor));
        }
        if (segment.y >= 0 && segment.y < maxY &&
            segment.x >= 0 && segment.x < maxX)
        {
            mvprintw(segment.y, segment.x, "%s", segment.getUnicodeChar());
            attroff(COLOR_PAIR(segment.isHead ? headColor : trailColor));
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

void Player::reset(int newX, int newY)
{
    if (newX != -1 && newY != -1)
    {
        x = newX;
        y = newY;
        startX = newX;
        startY = newY;
    }
    else
    {
        x = startX;
        y = startY;
    }

    direction = RIGHT;
    lastDirection = RIGHT;
    trail.clear();
    trail.push_back(TrailSegment(x, y, direction, direction, true));
}