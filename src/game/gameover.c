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

// Constants
static const float AMPLITUDE = 16.0f;
static const float WAVE_SPEED = 0.05f;
static const float GOVER_MAX = 60.0f;

// Bitmaps
static BITMAP* bmpGameover;

// Wave timer
static float waveTimer;
// Game over timer
static float goverTimer;


// Draw game over text
static void draw_game_over_text(int dx, int dy) {

    const int XOFF = 24;
    const int SPACE = 16;
    const float PERIOD = M_PI / 9.0f;

    int i = 0;
    int c = 0;
    int x = dx;
    int y = dy;

    for(; i < 10; ++ i) {

        if(i == 4) {

            x += SPACE;
            continue;
        }

        y = dy + (int)( floorf(sin(waveTimer + PERIOD*c) * AMPLITUDE) );

        if(goverTimer < GOVER_MAX) {

            int fade = 1 + (int)floorf( goverTimer / GOVER_MAX * 16.0f );
            draw_bitmap_region_fading(bmpGameover, c*29, 0,29,64, x, y, 0, fade, get_alpha());

        }
        else
            draw_bitmap_region(bmpGameover, c*29, 0,29,64, x, y, 0);

        x += XOFF;
        ++ c;
    }
}


// Initialize
void init_game_over(ASSET_PACK* ass) {

    // Get bitmaps
    bmpGameover = (BITMAP*)assets_get(ass, "gameOver");

    // (Re)set stuff
    gover_reset();
}


// Update
void gover_update(float tm) {

    if(!status_is_game_over()) return;

    // Update timer
    if(goverTimer < GOVER_MAX)
        goverTimer += 1.0f * tm;

    // Keyboard pressed (TEMP)
    if(vpad_get_button(2) == STATE_PRESSED ||
       vpad_get_button(0) == STATE_PRESSED ) {

        fade(1, 2.0f, game_reset);
        return;
    }

    // Update wave speed
    waveTimer += WAVE_SPEED * tm;
}


// Draw
void gover_draw() {

    if(!status_is_game_over()) return;

    // Draw game over text
    draw_game_over_text((256-240) / 2, 48);
}


// Reset game over
void gover_reset() {

    waveTimer = 0.0f;
    goverTimer = 0.0f;
}
