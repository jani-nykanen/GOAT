// GOAT
// Game over! screen (header)
// (c) 2018 Jani Nykänen

#include "gameover.h"

#include "game.h"
#include "status.h"

#include "../cursor.h"
#include "../vpad.h"
#include "../global.h"

#include "../leaderboard/menu.h"

#include "../include/audio.h"
#include "../include/renderer.h"
#include "../include/std.h"

// Constants
static const float AMPLITUDE = 16.0f;
static const float WAVE_SPEED = 0.05f;
static const float GOVER_MAX = 60.0f;
static const char* GOVER_TEXT[] = {
    "Retry", "Submit score", "Return to menu"
};
static const int ELEMENT_COUNT = 3;

// Bitmaps
static _BITMAP* bmpGameover;
static _BITMAP* bmpFont;
static _BITMAP* bmpFont2;
static _BITMAP* bmpFontBig;

// Samples
static SAMPLE* sAccept;
// static SAMPLE* sReject;
static SAMPLE* sSelect;

// Wave timer
static float waveTimer;
// Game over timer
static float goverTimer;

// Cursor
static CURSOR cursor;


// Change to title
static void change_to_title() {

    core_swap_scene("title");
}


// Draw menu text
static void draw_menu_text(int x, int y, int yoff) {

    int i = 0;
    for(; i < ELEMENT_COUNT; ++ i) {

        draw_text( (cursor.moving || (cursor.pos != i)) ? bmpFont : bmpFont2, 
            GOVER_TEXT[i],x,y +yoff*i,-7,0,false);
    }

}


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
            draw__BITMAP_region_fading(bmpGameover, c*29, 0,29,64, x, y, 0, fade, get_alpha());

        }
        else
            draw__BITMAP_region(bmpGameover, c*29, 0,29,64, x, y, 0);

        x += XOFF;
        ++ c;
    }
}


// Menu action
static void menu_action() {

    switch(cursor.pos) {

    // Retry
    case 0:
        fade(1, 2.0f, game_reset);
        break;


    // Submit score
    case 1:
        set_lb_menu_type(LB_MENU_SUBMIT);
        core_swap_scene("lbmenu");
        break;

    // Return
    case 2:
        fade(1, 2.0f, change_to_title);
        break;

    default:
        break;

    }

    play_sample(sAccept, 0.80f);
}


// Initialize
void init_game_over(ASSET_PACK* ass) {

    // Get Bitmaps
    bmpGameover = (_BITMAP*)assets_get(ass, "gameOver");
    bmpFont = (_BITMAP*)assets_get(ass, "font");
    bmpFont2 = (_BITMAP*)assets_get(ass, "font2");
    bmpFontBig = (_BITMAP*)assets_get(ass, "fontBig");

    sAccept = (SAMPLE*)assets_get(ass, "accept");
    sSelect = (SAMPLE*)assets_get(ass, "select");

    // (Re)set stuff
    gover_reset();
}


// Update
void gover_update(float tm) {

    if(!status_is_game_over()) return;

    // Update timer
    if(goverTimer < GOVER_MAX)
        goverTimer += 1.0f * tm;

    else {

        // Keyboard pressed
        if(vpad_get_button(2) == STATE_PRESSED ||
        vpad_get_button(0) == STATE_PRESSED ) {

            menu_action();
            return;
        }
    }

    // Update text wave
    waveTimer += WAVE_SPEED * tm;

    // Update cursor
    cursor_update(&cursor, tm);
}


// Draw
void gover_draw() {

    const int POS_X = 48;
    const int POS_Y = 192-48;
    const int YOFF = 14;
    const int BIG_TEXT_Y = 32;
    const int SCORE_Y = 192-80;

    if(!status_is_game_over()) return;

    // Draw game over text
    draw_game_over_text((256-240) / 2, BIG_TEXT_Y);

    // Translate, if not ready yet
    if(goverTimer < GOVER_MAX) {
        
        int p =  80 - (int)floorf(goverTimer / GOVER_MAX * 80.0f );
        translate(0, p);
    }

    // Draw menu text
    draw_menu_text(POS_X, POS_Y, YOFF);

    // Draw cursor
    cursor_draw(&cursor, POS_X, POS_Y, YOFF);

    // Draw score
    char scoreStr[16];
    status_get_score_string(scoreStr, 16);
    draw_text(bmpFontBig, scoreStr, 128, SCORE_Y, -16, 0, true);
}


// Reset game over
void gover_reset() {

    waveTimer = 0.0f;
    goverTimer = 0.0f;

    cursor = create_cursor(ELEMENT_COUNT);
}
