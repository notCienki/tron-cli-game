#include <ncurses.h>

int main() {
    initscr();              // Start ncurses
    noecho();               // Don't echo input
    curs_set(FALSE);        // Hide the cursor

    printw("Hello, Tron CLI!");
    refresh();              // Show output
    getch();                // Wait for key press

    endwin();               // Exit ncurses mode
    return 0;
}

