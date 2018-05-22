// GOAT
// Title screen scene (source)
// (c) 2018 Jani Nykänen

#include "title.h"

#include "../global.h"
#include "../vpad.h"

#include "../game/stage.h"
#include "../game/game.h"

#include "../include/std.h"
#include "../include/system.h"
#include "../include/renderer.h"
#include "../include/audio.h"

// Constants
static const float LOGO_AMPLITUDE = 12.0f;
static const float LOGO_WAVE_SPEED = 0.0625f;
static const float GO_AWAY_MAX = 60.0f;

// Bitmaps
static BITMAP* bmpFont;
static BITMAP* bmpFont2;
static BITMAP* bmpLogo;

// Samples
static SAMPLE* sAccept;
static SAMPLE* sReject;

// Logo wave timer
static float logoWave;
// Go away timer
static float goAway;
// Is going away
static bool goingAway;


// Draw logo
static void draw_logo(int dx, int dy) {

    const float PERIOD = M_PI / 6.0f;

    int w = bmpLogo->width / 4;

    int i = 0;
    int y;
    for(; i < 4; ++ i) {

        y = dy + (int)floorf(sinf(logoWave + PERIOD*i) * LOGO_AMPLITUDE);

        draw_bitmap_region(bmpLogo, i*w,0,w,bmpLogo->height, dx + w*i, y, 0);
    }

}


// Initialize
static int ts_init() {

    // Get assets
    ASSET_PACK* ass = global_get_asset_pack();
    
    bmpFont = (BITMAP*)assets_get(ass, "font");
    bmpFont2 = (BITMAP*)assets_get(ass, "font2"); 
    bmpLogo = (BITMAP*)assets_get(ass, "logo"); 

    sAccept = (SAMPLE*)assets_get(ass, "accept");
    sReject = (SAMPLE*)assets_get(ass, "reject");

    // Set defaults
    logoWave = 0.0f;
    goingAway = false;
    goAway = 0.0f;

    return 0;
}


// Update
static void ts_update(float tm) {


    // Update background
    stage_update_bg_only(tm);

    // Update waves
    logoWave += LOGO_WAVE_SPEED * tm;

    if(goingAway) {

        goAway += 1.0f * tm;
        if(goAway >= GO_AWAY_MAX) {

            game_reset();
            core_swap_scene("game");
        }
    }
    else {

        // Accept button down
        if(vpad_get_button(0) == STATE_PRESSED
        || vpad_get_button(2) == STATE_PRESSED) {

            goingAway = true;
            goAway = 0.0f;
            play_sample(sAccept, 0.80f);
        }

        // Quit
        if(vpad_get_button(3) == STATE_PRESSED) {

            fade(1,2.0f, core_terminate);
            play_sample(sReject, 0.70f);
        }
    }
}


// Draw
static void ts_draw() {

    // Draw stage background
    stage_draw();

    // Darken
    int dark = 4;
    if(goingAway) {

        dark = 4 - (int)floorf(goAway / GO_AWAY_MAX * 4.0f);
    }

    if(dark > 0)
        darken(dark);

    // Draw logo
    int y = 12;
    if(goingAway) {

        y = 12 - (int)floorf(goAway / GO_AWAY_MAX * 128.0f);
    }
    draw_logo(128-100, y);
}


// Swap
static void ts_on_change() {

    stage_reset();
    goingAway = false;
    goAway = 0.0f;
}


// Get leaderboard menu scene
SCENE ts_get_scene() {

    return scene_create(
        ts_init,ts_update,ts_draw,NULL,ts_on_change,
        "title");
}
