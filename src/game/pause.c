// GOAT
// Pause menu (header)
// (c) 2018 Jani Nykänen

#include "pause.h"

#include "game.h"

#include "../cursor.h"
#include "../vpad.h"
#include "../global.h"

#include "../include/renderer.h"
#include "../include/audio.h"

// Constants
static const char* PAUSE_TEXT[] = {
    "Resume", "Restart", "Full screen", "Audio: On", "Quit"
};
static const char* AUDIO_TEXT_OFF = "Audio: Off";
static const int PAUSE_BOX_W = 128;
static const int PAUSE_BOX_H = 88;
static const float DARK_INTERVAL = 4.0f;
static const int DARK_MAX = 4;
static const int ELEMENT_COUNT = 5;

// Bitmaps
static BITMAP* bmpFont;
static BITMAP* bmpFont2;

// Samples
static SAMPLE* sAccept;
static SAMPLE* sReject;
static SAMPLE* sSelect;

// Canvas copy
static FRAME* canvasCopy;

// Cursor
static CURSOR cursor;

// Dark timer
static float darkTimer;
// Dark count
static int darkCount;

// Is active
static bool active;


// Change to title
static void change_to_title() {

    core_swap_scene("title");
}


// Draw pause box
static void draw_pause_box(int x, int y, int w, int h) {

    fill_rect(x,y,w,h, 255);
    fill_rect(x+1,y+1,w-2,h-2, 0b01101101);
    fill_rect(x+2,y+2,w-4,h-4,0);
}


// Draw text
static void draw_pause_text(int x, int y, int yoff) {

    bool audioState = music_enabled() && samples_enabled();

    const char* text;

    int i = 0;
    for(; i < ELEMENT_COUNT; ++ i) {

        text = PAUSE_TEXT[i];
        if(i == 3 && !audioState)
            text = AUDIO_TEXT_OFF;

        draw_text( (cursor.moving || (cursor.pos != i)) ? bmpFont : bmpFont2, 
            text,x,y +yoff*i,-7,0,false);
    }
}


// Menu action
static void menu_action() {

    bool audioState = music_enabled() && samples_enabled();
    bool playSample = true;

    switch(cursor.pos) {

    // Resume
    case 0:
        active = false;
        break;

    // Restart
    case 1:
        fade(1, 2.0f, game_reset);
        active = false;
        break;

    // Full screen
    case 2:
        core_toggle_fullscreen();
        break;

    // Audio
    case 3:

        if(audioState) {

            enable_music(false);
            enable_samples(false);

            playSample = false;
        }
        else {

            enable_music(true);
            enable_samples(true);
        }

        break;

    // Quit
    case 4:
        fade(1,2.0f, change_to_title);
        active = false;
        break;

    default:
        break;

    }

    if(playSample) {

        play_sample(sAccept, 0.80f);
    }

}


// Initialize
int init_pause(ASSET_PACK* ass) {

    // Get bitmaps
    bmpFont = (BITMAP*)assets_get(ass, "font");
    bmpFont2 = (BITMAP*)assets_get(ass, "font2");

    sAccept = (SAMPLE*)assets_get(ass, "accept");
    sReject = (SAMPLE*)assets_get(ass, "reject");
    sSelect = (SAMPLE*)assets_get(ass, "select");

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

    // Update darkness
    if(darkCount < DARK_MAX) {

        darkTimer += 1.0f * tm;
        if(darkTimer >= DARK_INTERVAL) {

            ++ darkCount;
            darkTimer -= DARK_INTERVAL;
        }
    }

    cursor_update(&cursor, tm);

    // If pause button pressed, take action
    if(!cursor.moving && ( vpad_get_button(2) == STATE_PRESSED ||
       vpad_get_button(0) == STATE_PRESSED ) ) {

        menu_action();
        return;
    }

    // If escape, unpause
    if(vpad_get_button(3) == STATE_PRESSED) {

        play_sample(sReject, 0.70f);
        active = false;
        return;
    }
}



// Draw
void pause_draw() {

    const int YOFF = 14;
    const int POS_X = 128-PAUSE_BOX_W/2 + 18;
    const int POS_Y = 96-PAUSE_BOX_H/2 + 8; 

    // Draw canvas copy
    draw_bitmap_fast((BITMAP*)canvasCopy,0,0);

    // Darken
    darken(darkCount);

    // Draw box
    draw_pause_box(POS_X-18,POS_Y-8,  PAUSE_BOX_W, PAUSE_BOX_H);

    // Draw text
    draw_pause_text(POS_X, POS_Y,YOFF);

    // Draw cursor
    cursor_draw(&cursor, POS_X,POS_Y, YOFF);
}


// Is the game paused
bool pause_is_active() {

    return active;
}


// Active pause
void pause_active() {

    // Set values
    active = true;
    darkTimer = 0.0f;
    darkCount = 0;
    cursor = create_cursor(ELEMENT_COUNT);

    // Create a copy of the canvas
    frame_copy(get_global_frame(), canvasCopy);
}


// Destroy pause
void pause_destroy() {

    frame_destroy(canvasCopy);
}