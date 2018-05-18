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
    VEC2 speed;
    SPRITE spr;
    bool exist;
    bool hasGravity;

    float waveTimer;
    float deathTimer;
    float oldY;
}
GEM;

// Initialize gems
void init_gems(ASSET_PACK* ass);

// Create a gem
GEM create_gem(VEC2 pos);

// Create a gem with gravity
GEM create_gem_with_gravity(VEC2 pos, VEC2 speed);

// Update a gem
void gem_update(GEM* gem, float tm);

// Draw a gem
void gem_draw(GEM* gem);

// Gem-to-goat collision
void gem_goat_collision(GEM* gem, GOAT* g);

// Gem-to-floor collision
void gem_floor_collision(GEM* g, float x, float y, float w);

#endif // __GEM__


