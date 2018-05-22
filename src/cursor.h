// GOAT
// Cursor (header)
// (c) 2018 Jani Nykänen

#ifndef __CURSOR__
#define __CURSOR__

#include "include/std.h"
#include "include/system.h"

// Cursor type
typedef struct {

    int pos;
    int dir;
    bool moving;
    float moveTimer;
    float waveTimer;
    int elementCount;
}
CURSOR;

// Initialize cursor (i.e. load a bitmap)
void init_cursor(ASSET_PACK* ass);

// Create a cursor
CURSOR create_cursor(int ecount);

// Update cursor
void cursor_update(CURSOR* c, float tm);

// Draw a cursor
void cursor_draw(CURSOR* c, int x, int y, int yoff);

#endif // __CURSOR__
