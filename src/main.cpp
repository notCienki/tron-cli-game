#include "../include/game.h"
#include <ncurses.h>
using namespace std;

int main(int argc, char **argv)
{
    Game game(80, 24); // Create a game instance with terminal size
    game.init();       // Initialize the game
    game.run();        // Start the game loop
    return 0;          // Exit the program
}