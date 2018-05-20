// GOAT
// Global scene (header)
// (c) 2018 Jani Nykänen

#ifndef __GLOBAL__
#define __GLOBAL__

#include "engine/scene.h"
#include "engine/assets.h"

#include "include/std.h"

// Get assets
ASSET_PACK* global_get_asset_pack();

// Fade
void fade(int dir, float speed, void (*cb)(void));

// Is fading
bool is_fading();

// Get the global scene
SCENE global_get_scene();

#endif // __GLOBAL__

