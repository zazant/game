#include "config.h"
#include "application.h"

#include <iostream>

int main() {
  Config config;
  Application app(config);

  app.runLoop();

  return 0;
}
