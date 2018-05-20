// GOAT
// Stage (header)
// (c) 2018 Jani Nykänen

#ifndef __STAGE__
#define __STAGE__

#include "../include/system.h"

#include "goat.h"
#include "gem.h"

// Initialize stage
int stage_init(ASSET_PACK* ass);

// Update stage
void stage_update(float globalSpeed, float tm);

// Player collision
// TODO: void* => PLAYER*
void stage_pl_collision(void* pl, float tm);

// Draw stage
void stage_draw();

// Stage-to-goat collision
void stage_goat_collision(GOAT* g);

// Stage-to-gem collision
void stage_gem_collision(GEM* g);

// Reset the stage
void stage_reset();

#endif // __STAGE__
