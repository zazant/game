#include <iostream>

#include "config.h"
#include "game.h"

int main() {
    Config config;
    Game app(config);

    app.runLoop();

    return 0;
}