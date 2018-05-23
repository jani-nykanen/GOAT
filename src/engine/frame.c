// GOAT
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

    // Clear
    memset(f->data,0, (int)w*h);

    // Set dimensions
    f->width = w;
    f->height = h;

    return f;
}


// Destroy a frame
void frame_destroy(FRAME* f) {

    if(f == NULL) return;

    if(f->data != NULL)
        free(f->data);
        
    free(f);
}


// Copy the content of a frame to another
void frame_copy(FRAME* src, FRAME* dst) {

    if(src->width != dst->width || src->height != dst->height)
        return;

    memcpy(dst->data, src->data, src->width * src->height);
}
