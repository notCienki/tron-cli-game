#include "../include/core/GameEngine.h"

int main(int argc, char **argv)
{
    GameEngine engine;

    engine.initialize();
    engine.run();
    engine.shutdown();

    return 0;
}