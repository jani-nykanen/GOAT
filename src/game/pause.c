// <Insert project name here>
// Pause menu (header)
// (c) 2018 Jani Nykänen

#include "pause.h"

#include "../vpad.h"

#include "../include/renderer.h"

// Bitmaps
static BITMAP* bmpFontBig;
static BITMAP* bmpFontBig2;
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

// Is active
static bool active;


// Initialize
int init_pause(ASSET_PACK* ass) {

    // Get bitmaps
    bmpFontBig = (BITMAP*)assets_get(ass, "fontBig");
    bmpFontBig2 = (BITMAP*)assets_get(ass, "fontBig2");
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

    // If pause button pressed, make inactive (temp)
    if(vpad_get_button(2) == STATE_PRESSED) {

        active = false;
    }
}



// Draw
void pause_draw() {

    // Draw canvas copy
    draw_bitmap_fast((BITMAP*)canvasCopy,0,0);
}


// Is the game paused
bool pause_is_active() {

    return active;
}


// Active pause
void pause_active() {

    active = true;

    // Create a copy of the canvas
    frame_copy(get_global_frame(), canvasCopy);

    // Darken
    set_render_target(canvasCopy);
    darken(2);
    set_render_target(NULL);
}
