// GOAT
// Status (header)
// (c) 2018 Jani Nykänen

#include "status.h"

#include "../include/std.h"
#include "../include/renderer.h"

// Constants
static float HEALTH_FADE_MAX = 30.0f;
static int HEALTH_MAX = 3;
static int SCORE_BASE = 10;
static float HIDE_MAX = 30.0f;
static float APPEAR_MAX = 60.0f;
static float CONTROL_MAX = 180.0f;

// Status variables
static int health;
static float healthFadeTimer;
static int healthFade;
static unsigned int score;
static int coins;
static bool isGameOver;

// _BITMAP
static _BITMAP* bmpHUD;
static _BITMAP* bmpFont;
static _BITMAP* bmpFontBig;
static _BITMAP* bmpControls;

// Hide timer
static float hideTimer;
// Appear timer
static float appearTimer;
// Control timer
static float controlTimer;


// Get score string
static void get_score_string(char* buf, int bufLen) {

    char zeroes[6];

    // Add zeroes
    int i = 4;
    int p = 0;
    for(; score < (unsigned int)pow(10, i); -- i ) {

        zeroes[p ++] = '0';
    }
    zeroes[p] = '\0';

    // Create score string
    if(score > 0)
        snprintf(buf, bufLen, "%s%d", zeroes, score);

    else
        snprintf(buf, bufLen, "%s", zeroes); 
}


// Initialize status
void init_status(ASSET_PACK* ass) {

    // Get assets
    bmpHUD = (_BITMAP*)assets_get(ass, "HUD");
    bmpFont = (_BITMAP*)assets_get(ass, "font");
    bmpFontBig = (_BITMAP*)assets_get(ass, "fontBig");
    bmpControls = (_BITMAP*)assets_get(ass, "controls");

    // (Re)set variables
    reset_status();
}


// Reset status
void reset_status() {

    health = HEALTH_MAX;
    healthFadeTimer = 0.0f;
    healthFade = 0;
    score = 0;
    coins = 0;
    isGameOver = false;
    hideTimer = 0.0f;
    appearTimer = 0.0f;
    controlTimer = 0.0f;
}


// Update status
void status_update(float tm) {

    // Update control timer
    if(controlTimer < CONTROL_MAX) {

        controlTimer += 1.0f * tm;
    }

    // Update fading health
    if(healthFade != 0) {

        healthFadeTimer -= 1.0f * tm;
        if(healthFadeTimer <= 0.0f)
            healthFade = 0;
    }

    // Update hide timer (if game over)
    if(isGameOver && hideTimer < HIDE_MAX) {

        hideTimer += 1.0f * tm;
    }

    // Update appear timer
    if(appearTimer < APPEAR_MAX) {

        appearTimer += 1.0f * tm;
    }
}


// Draw status
void status_draw() {

    const int HEART_X = 0;
    const int HEART_Y = 0;
    const int HEART_DELTA = 22;
    const int SCORE_TEXT_Y = 0;
    const int SCORE_Y = 4;
    const int COIN_TEXT_Y = -4;
    const int COIN_Y = 0;
    const int FADE_COUNT = 8;

    int i = 0;
    int sx = 0;
    char str[16];

    // If game over & hiding
    if(isGameOver) {

        if(hideTimer < HIDE_MAX) {

            int p = -(int)floorf(hideTimer / HIDE_MAX * 32.0f);
            translate(0, p);
        }
        else
            return;
    }

    // If appear timer
    if(appearTimer < APPEAR_MAX) {

        int p = -32.0 + (int)floorf(appearTimer / APPEAR_MAX * 32.0f);
            translate(0, p);
    }

    int hmax = healthFade == 2 ? health-2 : health-1;

    // Draw hearts
    for(; i < HEALTH_MAX; ++ i) {

        sx = hmax >= i ? 24 : 0;

        draw_bitmap_region(bmpHUD, sx,0,24,24, 
            HEART_X + HEART_DELTA*i, HEART_Y, 0 );
    }

    // Draw a fading heart
    if(healthFade != 0) {

        int fade = 0;
        if(healthFade == 1)
            fade = 1 + (int)floorf(healthFadeTimer / HEALTH_FADE_MAX * FADE_COUNT);

        else
            fade = 1 + FADE_COUNT - (int)floorf(healthFadeTimer / HEALTH_FADE_MAX * FADE_COUNT);

        sx = 24;

        int hpos = healthFade == 1 ? health : health-1;

        draw_bitmap_region_fading(bmpHUD, sx,0,24,24, 
            HEART_X + HEART_DELTA*(hpos), HEART_Y, 0, fade, get_alpha() );
    }

    // Draw score
    get_score_string(str, 16);
    draw_text(bmpFont, " SCORE:", 128, SCORE_TEXT_Y, -6, 0, true);
    draw_text(bmpFontBig, str, 128, SCORE_Y, -16, 0, true);

    // Draw coins
    int coinX;
    if(coins < 10)
        coinX = 256-48;

    else if(coins < 100)
        coinX = 256-60;

    else
        coinX = 256-72;

    snprintf(str, 16, "~%d", coins);
    draw_text(bmpFontBig, str, coinX, COIN_TEXT_Y, -16, 0, false);
    draw_bitmap_region(bmpHUD,48,0,24,24, coinX -16, COIN_Y, 0);

    // Draw controls
    if(controlTimer < CONTROL_MAX) {

        translate(0, 0);

        int x = 0;
        if(controlTimer >= CONTROL_MAX -30.0f) {

            x = -(int)floorf( (controlTimer-(CONTROL_MAX-30.0f)) / 30.0f * 72.0f);
        }
        draw_bitmap(bmpControls, x, 0, 0);
    }

}


// Reduce health
void status_reduce_health() {

    if(health <= 0) return;

    if(-- health == 0) {

        isGameOver = true;
        return;
    }
    
    healthFadeTimer = HEALTH_FADE_MAX;
    healthFade = 1;

}


// Add health
void status_add_health() {

    if(health == HEALTH_MAX) return;

    health ++;
    healthFadeTimer = HEALTH_FADE_MAX;
    healthFade = 2;
}


// Add a coin
void status_add_coin() {

    ++ coins;
}


// Add score
void status_add_score() {

    score += SCORE_BASE + coins;
}


// Is the game over
bool status_is_game_over() {

    return isGameOver;
}


// Get score string
void status_get_score_string(char* buf, int len) {

    snprintf(buf, len, "SCORE: %d", (int)score);
}


// Get score
unsigned int status_get_score() {

    return score;
}
