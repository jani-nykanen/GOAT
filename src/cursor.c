// GOAT
// Cursor (source)
// (c) 2018 Jani Nykänen

#include "cursor.h"

#include "global.h"
#include "vpad.h"

#include "include/renderer.h"
#include "include/audio.h"

// Constants
static const float CURSOR_AMPLITUDE = 2.0f;
static const float CURSOR_WAVE_SPEED = 0.1f;
static const float MOVE_TIMER_MAX = 8.0f;

// Bitmaps
static BITMAP* bmpCursor;

// Samples
static SAMPLE* sSelect;


// Initialize cursor (i.e. load a bitmap)
void init_cursor(ASSET_PACK* ass) {

    // Get assets
    bmpCursor = (BITMAP*)assets_get(ass, "cursor");
    sSelect = (SAMPLE*)assets_get(ass, "select");
}


// Create a cursor
CURSOR create_cursor(int ecount) {

    CURSOR c;
    c.elementCount = ecount;
    c.pos = 0;
    c.waveTimer = 0.0f;
    c.moving = false;
    c.moveTimer = 0.0f;
    c.dir = 0;

    return c;
}


// Update cursor
void cursor_update(CURSOR* c, float tm) {

    const float DELTA = 0.5f;

    // Update cursor wave
    c->waveTimer += CURSOR_WAVE_SPEED * tm;

     // Move
    if(c->moving) {

        c->moveTimer += 1.0f * tm;
        if(c->moveTimer >= MOVE_TIMER_MAX)
            c->moving = false;
    }
    else {

        // Move cursor
        int oldPos = c->pos;
        VEC2 stick = vpad_get_stick();

        // Limit
        if(c->pos < c->elementCount-1 && stick.y > DELTA)
              ++ c->pos;

        else if(c->pos > 0 && stick.y < -DELTA)
             -- c->pos;

         // Set moving
        if(c->pos != oldPos) {

            c->moving = true;
            c->moveTimer = 0.0f;
            c->dir = c->pos > oldPos ? 1 : -1;

            play_sample(sSelect, 0.70f);
        }

    }
}


// Draw a cursor
void cursor_draw(CURSOR* c, int x, int y, int yoff) {

    // Draw cursor
    int cursorY = y + c->pos*yoff;
    if(c->moving) {

        cursorY = y + (int)floor((c->pos-c->dir)*yoff 
            + yoff*c->dir/MOVE_TIMER_MAX * c->moveTimer  );
    }

     draw_bitmap(bmpCursor,x-16 + (int)(sinf(c->waveTimer) * CURSOR_AMPLITUDE),
            cursorY, 0);
}

