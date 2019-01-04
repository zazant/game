#pragma once

struct Config {
    int WIDTH = 1200;
    int HEIGHT = 900;
    float FOV = 90.0f;
    float SENSITIVITY = 0.05f;
    struct {
        // makes it easier to reason about if it's an odd number (for finding the center)
        int CHUNK_SIZE = 171;
        float MOVEMENT_SPEED = 2.0f;
        float PLAYER_HEIGHT = 2.4f;
        bool FLY = false;
    } INTERNAL_SETTINGS;
};