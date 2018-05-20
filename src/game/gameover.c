// GOAT
// Game over! screen (header)
// (c) 2018 Jani Nykänen

#include "gameover.h"

#include "game.h"
#include "status.h"

#include "../vpad.h"
#include "../global.h"

#include "../include/renderer.h"
#include "../include/std.h"

// Bitmaps
static BITMAP* bmpGameover;


// Initialize
void init_game_over(ASSET_PACK* ass) {

    // Get bitmaps
    bmpGameover = (BITMAP*)assets_get(ass, "gameOver");
}


// Update
void gover_update(float tm) {

    if(!status_is_game_over()) return;

    // Keyboard pressed (TEMP)
    if(vpad_get_button(2) == STATE_PRESSED ||
       vpad_get_button(0) == STATE_PRESSED ) {

        fade(1, 2.0f, game_reset);
        return;
    }
}


// Draw
void gover_draw() {

    if(!status_is_game_over()) return;

    // TEMP
    draw_bitmap(bmpGameover, 0,64,0);
}