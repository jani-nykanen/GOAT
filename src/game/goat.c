// <Insert project name here>
// Goat (source)
// (c) 2018 Jani Nykänen

#include "goat.h"

#include "../include/std.h"

// Bitmaps
static BITMAP* bmpGoat;


// Initialize goats
void init_goat(ASSET_PACK* ass) {

    // Get assets
    bmpGoat = (BITMAP*)assets_get(ass, "goat");
}


// Create a new goat
GOAT create_goat(VEC2 p) {

    GOAT g;

    return g;
}


// Update goat
void goat_update(GOAT* g, float tm) {

}


// Draw goat
void goat_draw(GOAT* g) {

}
