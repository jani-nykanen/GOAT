// GOAT
// Configuration (header)
// (c) 2018 Jani Nykänen

#ifndef __CONFIG__
#define __CONFIG__

#include <SDL2/SDL.h>

#include "vector.h"

#include <stdbool.h>

// Caption buffer size
#define CAPTION_STRING_SIZE 64

// Asset path buffer size
#define ASSET_PATH_SIZE 128

// Configuration type
typedef struct {

    _POINT winSize;
    _POINT canvasSize;
    bool fullscreen;
    bool vsync;
    int frameRate;
    int musicVol;
    int sampleVol;
    char caption[CAPTION_STRING_SIZE];
    char assetPath[ASSET_PATH_SIZE];
    char keyconfPath[ASSET_PATH_SIZE];
}
CONFIG;

// Read a config file
int config_read(CONFIG* c, const char* path);

#endif // __CONFIG__