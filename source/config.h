#pragma once

struct Config {
    int WIDTH = 1200;
    int HEIGHT = 900;
    float MAX_FPS = 60.0f;
    float FOV = 90.0f;
    float SENSITIVITY = 0.05f;
    struct {
        // makes it easier to reason about if it's an odd number (for finding the center)
        int CHUNK_SIZE = 171;
        float MOVEMENT_SPEED = 10.0f;
        float RUN_MUL = 10.0f;
        float PLAYER_HEIGHT = 2.4f;
        bool FLY = true;
    } INTERNAL_SETTINGS;
};