#include "../include/game.h"
#include "../include/menu.h"
#include <ncurses.h>
using namespace std;

int main(int argc, char **argv)
{
    // Unicode support setup (jak w Game::init())
    setlocale(LC_ALL, "");

    printf("\033[?1049h\033[H");
    fflush(stdout);

    // Initialize ncurses
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, FALSE); // Blocking input for menu

    Menu menu;
    menu.init();

    while (true)
    {
        menu.render();

        if (menu.handleInput())
        {
            if (menu.shouldQuit())
            {
                break;
            }
            else if (menu.shouldStartGame())
            {
                // Start the game
                Game game(80, 24);
                game.init();
                game.setGameSpeed(menu.getGameSpeed());
                game.run();
                // When game ends, return to menu
                menu.setState(MAIN_MENU);
                nodelay(stdscr, FALSE);
            }
        }
    }

    endwin();
    return 0;
}