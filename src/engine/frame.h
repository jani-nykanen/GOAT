// <Insert project name here>
// Frame (header)
// (c) 2018 Jani Nykänen

#ifndef __FRAME__
#define __FRAME__

#include <SDL2/SDL.h>

// Frame type
typedef struct {

    Uint8* data;
    Uint16 width;
    Uint16 height;

}
FRAME;

// Create a frame
FRAME* frame_create(Uint16 w, Uint16 h);

// Destroy a frame
void frame_destroy(FRAME* f);

#endif // __FRAME__