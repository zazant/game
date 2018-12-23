#pragma once

#include "player.h"

class World {
public:
  World();

  update();

  render();
private:
  Player *player;

  float *vertices;
