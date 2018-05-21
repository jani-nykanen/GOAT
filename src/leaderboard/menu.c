// GOAT
// Leaderboard menu scene (header)
// (c) 2018 Jani Nykänen

#include "menu.h"

#include "../global.h"
#include "../vpad.h"

#include "../include/std.h"
#include "../include/system.h"
#include "../include/renderer.h"

// Constants
static const float DARK_MAX = 4;
static const int DARK_INTERVAL = 4.0f;

// Canvas copy
static FRAME* canvasCopy;
// Dark timer
static float darkTimer;
// Dark count
static int darkCount;

// Bitmaps
static BITMAP* bmpFont;
static BITMAP* bmpFont2;

// Mode
static int mode;


// Draw input box
static void draw_input_box(int x, int y, int w, int h) {

    fill_rect(x,y,w,h, 255);
    fill_rect(x+1,y+1,w-2,h-2, 0b01101101);
    fill_rect(x+2,y+2,w-4,h-4,0);
}


// Initialize
static int lb_menu_init() {

    // Get assets
    ASSET_PACK* ass = global_get_asset_pack();
    bmpFont = (BITMAP*)assets_get(ass, "font");
    bmpFont2 = (BITMAP*)assets_get(ass, "font2"); 

    // Set defaults
    mode = 0;
    darkTimer = 0.0f;
    darkCount = 0;

    // Create canvas
    canvasCopy = frame_create(256, 192);
    if(canvasCopy == NULL)
        return 1;

    return 0;
}


// Update
static void lb_menu_update(float tm) {

    // Update darkness
    if(darkCount < DARK_MAX ){

        darkTimer += 1.0f * tm;
        if(darkTimer >= DARK_INTERVAL) {

            darkTimer -= DARK_INTERVAL;
            ++ darkCount;
        }
    }    
}


// Draw
static void lb_menu_draw() {

    const int SUBMIT_W = 128;
    const int SUBMIT_H = 64;

    // Draw canvas copy
    draw_bitmap_fast(canvasCopy, 0, 0);

    // Darken
    darken(darkCount);

    // Submit
    if(mode == LB_MENU_SUBMIT) {

        draw_input_box(
            128 - SUBMIT_W/2,
            96 - SUBMIT_H/2,
            SUBMIT_W,
            SUBMIT_H 
        );
    }
}


// Swap
static void lb_menu_on_change() {

    darkTimer = 0.0f;
    darkCount = 0;

    // Create a copy of the canvas
    frame_copy(get_global_frame(), canvasCopy);
}


// Set menu type
void set_lb_menu_type(int type) {

    mode = type;
}


// Get leaderboard menu scene
SCENE lb_menu_get_scene() {

    return scene_create(
        lb_menu_init,lb_menu_update,lb_menu_draw,NULL,lb_menu_on_change,
        "lbmenu");
}
