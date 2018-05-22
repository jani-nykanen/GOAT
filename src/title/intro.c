// GOAT
// Title screen scene (source)
// (c) 2018 Jani Nykänen

#include "intro.h"

#include "../global.h"

#include "../include/std.h"
#include "../include/system.h"
#include "../include/renderer.h"
#include "../include/audio.h"

// Constants
static const float INTRO_TIME_INVERVAL= 120.0f;
static const float DMAX = 30.0f;

// Bitmaps
static _BITMAP* bmpIntro;

// Music
static MUSIC* mTheme;

// Timer
static float timer;
// Phase
static int phase;


// Initialize
static int intro_init() {

    // Get assets
    ASSET_PACK* ass = global_get_asset_pack();
    bmpIntro = (_BITMAP*)assets_get(ass, "intro");
    mTheme = (MUSIC*)assets_get(ass, "theme");

    // Set defaults
    timer = 0.0f;
    phase = 0;

    return 0;
}


// Update
static void intro_update(float tm) {

    if(is_fading()) return;

    timer += 1.0f * tm;
    if(timer >= INTRO_TIME_INVERVAL) {

        timer -= INTRO_TIME_INVERVAL;
        ++ phase;
        if(phase == 2) {

            // Start music
            fade_in_music(mTheme, 0.50f, -1, 1000);

            core_swap_scene("title");
            fade(0, 2.0f, NULL);
        }
    }
}


// Draw
static void intro_draw() {

    if(is_fading()) return;

    int sy = phase == 0 ? 0 : 96;

    int dvalue = 0;
    if(timer < DMAX) {

        dvalue = (int)((1.0f-timer/DMAX) * 14.0f);
    }
    else if(timer >= INTRO_TIME_INVERVAL - DMAX) {

        dvalue = (int)( (timer- (INTRO_TIME_INVERVAL - DMAX) ) /DMAX  * 14.0f);
    }

    draw__BITMAP_region(bmpIntro, 0, sy, 144, 96,
        128 - 72, 96-48, 0);

    if(dvalue > 0)
        darken(dvalue);
}


// Get leaderboard menu scene
SCENE intro_get_scene() {

    return scene_create(
        intro_init,intro_update,intro_draw,NULL,NULL,
        "intro");
}
