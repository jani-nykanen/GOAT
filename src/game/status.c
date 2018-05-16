// <Insert project name here>
// Status (header)
// (c) 2018 Jani Nykänen

#include "status.h"

#include "../include/std.h"
#include "../include/renderer.h"

// Constants
static float HEALTH_FADE_MAX = 30.0f;
static int HEALTH_MAX = 3;

// Status variables
static int health;
static float healthFadeTimer;
static bool healthFade;
static unsigned int score;
static int coins;

// Bitmap
static BITMAP* bmpHUD;
static BITMAP* bmpFont;
static BITMAP* bmpFontBig;


// Initialize status
void init_status(ASSET_PACK* ass) {

    // Get assets
    bmpHUD = (BITMAP*)assets_get(ass, "HUD");
    bmpFont = (BITMAP*)assets_get(ass, "font");
    bmpFontBig = (BITMAP*)assets_get(ass, "fontBig");

    // (Re)set variables
    reset_status();
}


// Reset status
void reset_status() {

    health = HEALTH_MAX;
    healthFadeTimer = 0.0f;
    healthFade = false;
    score = 0;
    coins = 0;
}


// Update status
void status_update(float tm) {

    // Update fading health
    if(healthFade) {

        healthFadeTimer -= 1.0f * tm;
        if(healthFadeTimer <= 0.0f)
            healthFade = false;
    }
}


// Draw status
void status_draw() {

    const int HEART_X = 0;
    const int HEART_Y = 0;
    const int HEART_DELTA = 22;
    const int SCORE_TEXT_Y = 0;
    const int SCORE_Y = 4;
    const int COIN_TEXT_X = 256-60;
    const int COIN_TEXT_Y = -4;
    const int COIN_X = COIN_TEXT_X - 16;
    const int COINT_Y = 0;

    int i = 0;
    int sx = 0;

    // Draw hearts
    for(; i < HEALTH_MAX; ++ i) {

        sx = health >= i ? 24 : 0;

        draw_bitmap_region(bmpHUD, sx,0,24,24, 
            HEART_X + HEART_DELTA*i, HEART_Y, 0 );
    }

    // Draw score
    draw_text(bmpFont, " SCORE:", 128, SCORE_TEXT_Y, -6, 0, true);
    draw_text(bmpFontBig, "00000", 128, SCORE_Y, -15, 0, true);

    // Draw coins
    char str[16];
    snprintf(str, 16, "~%d", coins);
    draw_text(bmpFontBig, str, COIN_TEXT_X, COIN_TEXT_Y, -15, 0, false);
    draw_bitmap_region(bmpHUD,48,0,24,24, COIN_X, COINT_Y, 0);

}


// Reduce health
void status_reduce_health() {

    -- health;
    healthFadeTimer = HEALTH_FADE_MAX;
}


// Add a coin
void status_add_coin() {

    ++ coins;
}


// Add score
void status_add_score() {

    ++ score;
}
