#include "../include/game.h"
#include "../include/menu.h"
#include <ncurses.h>
using namespace std;

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "");

    printf("\033[?1049h\033[H");
    fflush(stdout);

    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, FALSE);

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
                Game game(80, 24);
                game.init();
                game.setGameSpeed(menu.getGameSpeed());
                game.setColorScheme(menu.getColorScheme());
                game.setGameMode(menu.getGameMode());
                game.run();
                menu.setState(MAIN_MENU);
                nodelay(stdscr, FALSE);
            }
        }
    }

    endwin();
    return 0;
}