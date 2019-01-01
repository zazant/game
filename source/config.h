#pragma once

struct Config {
    int WIDTH = 1200;
    int HEIGHT = 900;
    float FOV = 90.0f;
    float SENSITIVITY = 0.05f;
    struct internal_settings {
        int CHUNK_SIZE = 17;
        float MOVEMENT_SPEED = 2.0f;
        float PLAYER_HEIGHT = 2.0f;
    } INTERNAL_SETTINGS;
};