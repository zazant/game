#include <iostream>

#include "config.h"
#include "game.h"

int main()
{
    // TODO: add config.ini
    Config config;
    Game app(config);

    app.runLoop();

    return 0;
}