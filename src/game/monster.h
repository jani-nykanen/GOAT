// GOAT
// Monster (header)
// (c) 2018 Jani Nykänen

#ifndef __MONSTER__
#define __MONSTER__

#include "../include/renderer.h"
#include "../include/system.h"

#include "goat.h"

// Monster type
typedef struct {

    VEC2 pos;
    VEC2 startPos;
    VEC2 speed;
    VEC2 deathSpeed;
    VEC2 target;

    float spcTimer;
    bool spcSwitch;
    
    int id;
    int flip;
    int direction;
    SPRITE spr;

    SPRITE splash;
    VEC2 splashPos;

    float leftLimit;
    float rightLimit;

    bool exist;
    bool stomped;
    bool dying;
    float deathTimer;
}
MONSTER;

// Initialize monsters
void init_monsters(ASSET_PACK* ass);

// Create a monster
MONSTER create_monster(VEC2 pos, float left, float right, int id);

// Update a monster
void monster_update(MONSTER* m, float tm);

// Draw a monster
void monster_draw(MONSTER* m);

// Monster-to-goat collision
void monster_goat_collision(MONSTER* m, GOAT* g);

// Monster-to-monster collision
void monster_to_monster_collision(MONSTER* m1, MONSTER* m2);

#endif // __MONSTER__
