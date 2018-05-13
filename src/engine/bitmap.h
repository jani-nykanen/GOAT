// <Insert project name here>
// Bitmap (header)
// (c) 2018 Jani Nykänen

#ifndef __BITMAP__
#define __BITMAP__

#include "frame.h"

// Bitmap type
typedef FRAME BITMAP;

// Load a bitmap
BITMAP* bitmap_load(const char* path);

// Destroy bitmap
#define bitmap_destroy(b) frame_destroy((FRAME*)b)

// Create a bitmap
#define bitmap_create(w, h) (BITMAP*)frame_create(w, h)

#endif // __BITMAP__