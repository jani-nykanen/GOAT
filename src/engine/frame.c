// <Insert project name here>
// Frame (source)
// (c) 2018 Jani Nykänen

#include "frame.h"

#include "error.h"

#include <string.h>
#include <stdlib.h>


// Create a frame
FRAME* frame_create(Uint16 w, Uint16 h) {

    // Allocate memory for data
    Uint8* data = (Uint8*)malloc(w*h * sizeof(Uint8));
    if(data == NULL) {

        error_mem_alloc();
        return NULL;
    }

    // Allocate memory for the frame
    FRAME* f = (FRAME*)malloc(sizeof(FRAME));
    if(f == NULL) {

        error_mem_alloc();
        return NULL;
    }

    // Set data
    f->data = data;

    // Set dimensions
    f->width = w;
    f->height = h;

    return f;
}


// Destroy a frame
void frame_destroy(FRAME* f) {

    if(f == NULL) return;

    free(f->data);
    free(f);
}