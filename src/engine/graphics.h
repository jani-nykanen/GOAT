// GOAT
// Graphics routines (header)
// (c) 2018 Jani Nykänen

#ifndef __GRAPHICS__
#define __GRAPHICS__

#include "frame.h"
#include "bitmap.h"

#include "vector.h"

#include <SDL2/SDL.h>

#include <stdbool.h>

// Flipping flags
enum {

    FLIP_NONE = 0,
    FLIP_H = 1,
    FLIP_V = 2,
};


// Initialize
void graphics_init(SDL_Renderer* rend);

// Bind frame
void bind_frame(FRAME* f);

// Clear screen
void clear(Uint8 color);

// Set alpha
void set_alpha(Uint8 color);

// Get alpha
Uint8 get_alpha();

// Use frame to create a canvas texture
int create_canvas_texture(FRAME* f);

// Update canvas texture
void update_canvas_texture();

// Draw canvas texture
void draw_canvas_texture(POINT pos, POINT size);

// Fill rectangle
void fill_rect(int x, int y, int w, int h, Uint8 color);

// Draw a bitmap
void draw_bitmap(BITMAP* bmp, int x, int y, int flip);

// Draw a bitmap region
void draw_bitmap_region(BITMAP* bmp, int sx, int sy, int sw, int sh, int dx, int dy, int flip);

// Draw a "fading" bitmap
void draw_bitmap_region_fading(BITMAP* bmp, int sx, int sy, int sw, int sh, int dx, int dy, int flip, int fade, Uint8 color);

// Faster routine for drawing a bitmap (no alpha or flipping)
void draw_bitmap_fast(BITMAP* bmp, int x, int y);

// Faster routine for drawing a bitmap region (no alpha or flipping)
void draw_bitmap_region_fast(BITMAP* bmp, int sx, int sy, int sw, int sh, int dx, int dy);

// Draw text with a bitmap font
void draw_text(BITMAP* font, const char* text, int x, int y, int xoff, int yoff, bool center);

// Draw a triangle
void draw_inverse_triangle(int x1, int y1, int x2, int y2, int x3, int y3);

// Draw a line
void draw_line(int x1, int y1, int x2, int y2, Uint8 color);

// Draw a triangle
void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, Uint8 color);

// Get the global frame
FRAME* get_global_frame();

// Translate
void translate(int x, int y);

// Set the render target
void set_render_target(BITMAP* target);

// Darken the whole screen
void darken(int d);

// Set uv coordinates
void set_uv_coords(float u1, float v1, float u2, float v2, float u3, float v3);

// Bind a texture
void bind_texture(BITMAP* tex);

#endif // __GRAPHICS__
