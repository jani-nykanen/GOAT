// <Insert project name here>
// Pause menu (header)
// (c) 2018 Jani Nykänen

#include "pause.h"

#include "game.h"

#include "../vpad.h"
#include "../global.h"

#include "../include/renderer.h"

// Constants
static const char* PAUSE_TEXT[] = {
    "Resume", "Restart", "Full screen", "Audio", "Quit"
};
static const int PAUSE_BOX_W = 128;
static const int PAUSE_BOX_H = 88;
static const float CURSOR_AMPLITUDE = 2.0f;
static const float CURSOR_WAVE_SPEED = 0.1f;
static const float MOVE_TIMER_MAX = 8.0f;

// Bitmaps
static BITMAP* bmpFont;
static BITMAP* bmpFont2;
static BITMAP* bmpCursor;

// Canvas copy
static FRAME* canvasCopy;

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

// Is active
static bool active;


// Draw pause box
static void draw_pause_box(int x, int y, int w, int h) {

    fill_rect(x,y,w,h, 255);
    fill_rect(x+1,y+1,w-2,h-2, 0b01101101);
    fill_rect(x+2,y+2,w-4,h-4,0);
}


// Draw text
static void draw_pause_text(int x, int y, int yoff) {

    int i = 0;
    for(; i < 5; ++ i) {

        draw_text( (moving || (cursorPos != i)) ? bmpFont : bmpFont2, 
            PAUSE_TEXT[i],x,y +yoff*i,-7,0,false);
    }
}


// Menu action
static void menu_action() {

    switch(cursorPos) {

    // Resume
    case 0:
        active = false;
        return;

    // Restart
    case 1:
        fade(1, 2.0f, game_reset);
        active = false;
        return;

    // Quit
    case 4:
        fade(1,2.0f, core_terminate);
        active = false;
        return;

    default:
        break;

    }

}


// Initialize
int init_pause(ASSET_PACK* ass) {

    // Get bitmaps
    bmpFont = (BITMAP*)assets_get(ass, "font");
    bmpFont2 = (BITMAP*)assets_get(ass, "font2");
    bmpCursor = (BITMAP*)assets_get(ass, "cursor");

    // Set defaults
    active = false;

    // Create the "canvas copy"
    FRAME* fr = get_global_frame();
    canvasCopy = frame_create(fr->width, fr->height);
    if(canvasCopy == NULL) {

        return 1;
    }

    return 0;
}


// Update
void pause_update(float tm) {

    const float DELTA = 0.5f;

    // Update cursor wave
    cursorWave += CURSOR_WAVE_SPEED * tm;

    // Move
    if(moving) {

        moveTimer += 1.0f * tm;
        if(moveTimer >= MOVE_TIMER_MAX)
            moving = false;
        else
            return;
    }

    // If pause button pressed, make inactive (temp)
    if(vpad_get_button(2) == STATE_PRESSED ||
       vpad_get_button(0) == STATE_PRESSED ) {

        menu_action();
        return;
    }

    // Move cursor
    int oldPos = cursorPos;
    VEC2 stick = vpad_get_stick();

    // Limit
    if(cursorPos < 4 && stick.y > DELTA)
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
void pause_draw() {

    const int YOFF = 14;

    int x = 128-PAUSE_BOX_W/2;
    int y = 96-PAUSE_BOX_H/2;

    // Draw canvas copy
    draw_bitmap_fast((BITMAP*)canvasCopy,0,0);

    // Draw box
    draw_pause_box(x,y,  PAUSE_BOX_W, PAUSE_BOX_H);

    // Draw text
    draw_pause_text(x + 18, y + 8,YOFF);

    // Draw cursor
    int cursorY = y + 8 + cursorPos*YOFF;
    if(moving) {

        cursorY = y + 8 + (int)floor((cursorPos-cursorDir)*YOFF + YOFF*cursorDir/MOVE_TIMER_MAX * moveTimer  );
    }

    draw_bitmap(bmpCursor,x + 4 + (int)(sinf(cursorWave) * CURSOR_AMPLITUDE),
         cursorY, 0);
}


// Is the game paused
bool pause_is_active() {

    return active;
}


// Active pause
void pause_active() {

    // Set values
    active = true;
    cursorPos = 0;
    cursorWave = 0.0f;
    moving = false;
    moveTimer = 0.0f;

    // Create a copy of the canvas
    frame_copy(get_global_frame(), canvasCopy);

    // Darken
    set_render_target(canvasCopy);
    darken(2);
    set_render_target(NULL);
}
