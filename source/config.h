#pragma once

struct Config {
    int WIDTH = 1000;
    int HEIGHT = 1000;
    float FOV = 80.0f;
    float SENSITIVITY = 0.05f;
    struct internal_settings {
        int CHUNK_SIZE = 17;
        float MOVEMENT_SPEED = 1.0f;
    } INTERNAL_SETTINGS;
};