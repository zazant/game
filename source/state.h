#pragma once

#include "world.h"
#include "player.h"

class State {
public:
  State();

  update(float deltaTime);

  render();
private:
  World mWorld;

  player mPlayer;
}
