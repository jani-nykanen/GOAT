// <Insert project name here>
// Goat (header)
// (c) 2018 Jani Nykänen

#ifndef __GOAT__
#define __GOAT__

#include "../include/renderer.h"
#include "../include/system.h"

// Goat type
typedef struct {

    VEC2 pos;
    VEC2 speed;
    VEC2 target;
    float oldY;
    float dashTimer;

    SPRITE spr;
    int flip;

    bool canJump;
    bool dashing;
}
GOAT;

// Initialize goats
void init_goat(ASSET_PACK* ass);

// Create a new goat
GOAT create_goat(VEC2 p);

// Update goat
void goat_update(GOAT* g, float tm);

// Draw goat
void goat_draw(GOAT* g);

// Goat-to-floor collision
void goat_floor_collision(GOAT* g, float x, float y, float w);

#endif // __GOAT__
