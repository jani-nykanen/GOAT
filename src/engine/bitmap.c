// <Insert project name here>
// Bitmap (source)
// (c) 2018 Jani Nykänen

#include "bitmap.h"

#include "graphics.h"

#include "error.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"

// Load a bitmap
BITMAP* bitmap_load(const char* path) {

    // Allocate memory
    BITMAP* bmp = (BITMAP*)malloc(sizeof(BITMAP));
    if(bmp == NULL) {

        error_mem_alloc();
        return NULL;
    }

    int comp, w, h;

    // Load image
    Uint8* pdata = stbi_load(path,&w,&h,&comp,4);
    if(pdata == NULL) {

        error_throw("Failed to load a bitmap in ",path);
        return NULL;
    }

    unsigned int pixelCount = w * h;

    // Allocate image and temp buffer data
    bmp->data = (Uint8*)malloc(sizeof(Uint8) * pixelCount);
    if(bmp->data == NULL)  {

        error_mem_alloc();
        return NULL;
    }

    // Go through the data
    int i = 0;
    Uint8 pixel;
    Uint8 r,g,b,a;
    Uint8 er,eg,eb;

    // Convert pixel data to RGBA332
    for(; i < pixelCount; i++) {

        a = pdata[i*4 +3];
        if(a < 255) {
            
            bmp->data[i] = get_alpha();;
            continue;
        }

        b = pdata[i*4 +2];
        g = pdata[i*4 +1];
        r = pdata[i*4 ];

        er = (Uint8) round(r / 36.428f);
        if(er > 7) r = 7;
        er = er << 5;
        eg = (Uint8) round(g / 36.428f);
        if(eg > 7) eg = 7;
        eg = eg << 2;
        eb = (b / 85);

        pixel = er | eg | eb;

        bmp->data[i] = pixel;
    }

    // Store dimensions
    bmp->width = (Uint16)w;
    bmp->height = (Uint16)h;

    // Free data
    stbi_image_free(pdata);

    return bmp;
}