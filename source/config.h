#pragma once

struct Config {
    int WIDTH = 1000;
    int HEIGHT = 1000;
    float FOV = 80.0f;
    float SENSITIVITY = 0.05f;
    struct world_settings {
        int CHUNK_SIZE = 17;
    } WORLD_SETTINGS;
};