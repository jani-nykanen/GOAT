// GOAT
// Game over! screen (header)
// (c) 2018 Jani Nykänen

#include "gameover.h"

#include "game.h"
#include "status.h"

#include "../vpad.h"
#include "../global.h"

#include "../leaderboard/menu.h"

#include "../include/renderer.h"
#include "../include/std.h"

// Constants
static const float AMPLITUDE = 16.0f;
static const float WAVE_SPEED = 0.05f;
static const float GOVER_MAX = 60.0f;
static const float CURSOR_AMPLITUDE = 2.0f;
static const float CURSOR_WAVE_SPEED = 0.1f;
static const float MOVE_TIMER_MAX = 8.0f;
static const char* GOVER_TEXT[] = {
    "Retry", "Submit score", "Return to menu"
};
static const int ELEMENT_COUNT = 3;

// Bitmaps
static BITMAP* bmpGameover;
static BITMAP* bmpFont;
static BITMAP* bmpFont2;
static BITMAP* bmpCursor;

// Wave timer
static float waveTimer;
// Game over timer
static float goverTimer;

// Cursor pos
static int cursorPos;
// Cursor wave
static float cursorWave;
// Cursor movement timer
static float moveTimer;
// Is the cursor moving
static bool moving;
// Cursor direction
static int cursorDir;


// Draw menu text
static void draw_menu_text(int x, int y, int yoff) {

    int i = 0;
    for(; i < ELEMENT_COUNT; ++ i) {

        draw_text( (moving || (cursorPos != i)) ? bmpFont : bmpFont2, 
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
            draw_bitmap_region_fading(bmpGameover, c*29, 0,29,64, x, y, 0, fade, get_alpha());

        }
        else
            draw_bitmap_region(bmpGameover, c*29, 0,29,64, x, y, 0);

        x += XOFF;
        ++ c;
    }
}


// Menu action
static void menu_action() {

    switch(cursorPos) {

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
        fade(1, 2.0f, core_terminate);
        break;

    default:
        break;

    }
}


// Initialize
void init_game_over(ASSET_PACK* ass) {

    // Get bitmaps
    bmpGameover = (BITMAP*)assets_get(ass, "gameOver");
    bmpFont = (BITMAP*)assets_get(ass, "font");
    bmpFont2 = (BITMAP*)assets_get(ass, "font2");
    bmpCursor = (BITMAP*)assets_get(ass, "cursor");

    // (Re)set stuff
    gover_reset();
}


// Update
void gover_update(float tm) {

    const float DELTA = 0.5f;

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

        // Update cursor wave
        cursorWave += CURSOR_WAVE_SPEED * tm;
    }

    // Move
    if(moving) {

        moveTimer += 1.0f * tm;
        if(moveTimer >= MOVE_TIMER_MAX)
            moving = false;
        else
            return;
    }

    // Update text wave
    waveTimer += WAVE_SPEED * tm;

    // Move cursor
    int oldPos = cursorPos;
    VEC2 stick = vpad_get_stick();

    // Limit
    if(cursorPos < ELEMENT_COUNT-1 && stick.y > DELTA)
        ++ cursorPos;

    else if(cursorPos > 0 && stick.y < -DELTA)
        -- cursorPos;

    // Set moving
    if(cursorPos != oldPos) {

        moving = true;
        moveTimer = 0.0f;
        cursorDir = cursorPos > oldPos ? 1 : -1;
    }
}


// Draw
void gover_draw() {

    const int POS_X = 48;
    const int POS_Y = 192-56;
    const int YOFF = 14;

    if(!status_is_game_over()) return;

    // Draw game over text
    draw_game_over_text((256-240) / 2, 48);

    // Translate, if not ready yet
    if(goverTimer < GOVER_MAX) {
        
        int p =  56 - (int)floorf(goverTimer / GOVER_MAX * 56.0f );
        translate(0, p);
    }

    // Draw menu text
    draw_menu_text(POS_X, POS_Y, YOFF);

    // Draw cursor
    int cursorY = POS_Y + cursorPos*YOFF;
    if(moving) {

        cursorY = POS_Y + (int)floor((cursorPos-cursorDir)*YOFF + YOFF*cursorDir/MOVE_TIMER_MAX * moveTimer  );
    }

    draw_bitmap(bmpCursor,POS_X-16 + (int)(sinf(cursorWave) * CURSOR_AMPLITUDE),
         cursorY, 0);
}


// Reset game over
void gover_reset() {

    waveTimer = 0.0f;
    goverTimer = 0.0f;

    moving = false;
    cursorDir = 0;
    cursorPos = 0;
    cursorWave = 0.0f;
}
