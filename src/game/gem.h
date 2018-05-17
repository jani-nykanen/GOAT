// <Insert project name here>
// Gem (header)
// (c) 2018 Jani Nykänen

#ifndef __GEM__
#define __GEM__

#include "../include/renderer.h"
#include "../include/system.h"

#include "goat.h"

// Gem type
typedef struct {

    VEC2 pos;
    SPRITE spr;
    bool exist;

    float waveTimer;
    float deathTimer;
}
GEM;

// Initialize gems
void init_gems(ASSET_PACK* ass);

// Create a gem
GEM create_gem(VEC2 pos);

// Update a gem
void gem_update(GEM* gem, float tm);

// Draw a game
void gem_draw(GEM* gem);

// Gem-to-goat collision
void gem_goat_collision(GEM* gem, GOAT* g);

#endif // __GEM__


