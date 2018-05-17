// <Insert project name here>
// Gem (source)
// (c) 2018 Jani Nykänen

#include "gem.h"

#include "status.h"
#include "camera.h"

#include "../include/std.h"

// Constants
static const float AMPLITUDE = 4.0f;
static const float WAVE_SPEED = 0.05f;
static const float DEATH_MAX = 30.0f;

// Bitmaps
static BITMAP* bmpGem;


// Initialize gems
void init_gems(ASSET_PACK* ass) {

    // Get bitmaps
    bmpGem = (BITMAP*)assets_get(ass, "gem");
}


// Create a gem
GEM create_gem(VEC2 pos) {
    
    GEM g;
    g.pos = pos;
    g.spr = create_sprite(24, 24);
    g.waveTimer = (float)(rand() % 1000) / 1000.0f * 2 * M_PI;
    g.deathTimer = 0.0f;
    g.exist = true;

    return g;
}


// Update a gem
void gem_update(GEM* gem, float tm) {

    if(!gem->exist) {

        // Update death timer
        if(gem->deathTimer > 0.0f) {

            gem->deathTimer -= 1.0f * tm;
        }

        return;   
    }

    // Wave
    gem->waveTimer += WAVE_SPEED * tm;

    // Animate
    spr_animate(&gem->spr,0,0,4, 5, tm);

    // If outside the screen, "kill"
    if(gem->pos.y < get_global_camera()->pos.y - 12.0f - AMPLITUDE) {

        gem->exist = false;
    }
}


// Draw a game
void gem_draw(GEM* gem) {

    // Rendering position
    int y = (int)round(gem->pos.y-12.0f + AMPLITUDE * sin(gem->waveTimer));
    int x = (int)round(gem->pos.x-12.0f);

    if(!gem->exist) {

        // Fading
        if(gem->deathTimer > 0.0f) {

            int fade = 1+ (int)round(gem->deathTimer / DEATH_MAX * 8.0f);
            draw_bitmap_region_fading(bmpGem,0,0,24,24,x,y, FLIP_NONE, fade, get_alpha());
        }   
        return;
    }

    // Draw
    spr_draw(&gem->spr,bmpGem, x,y, FLIP_NONE);
}


// Gem-to-goat collision
void gem_goat_collision(GEM* gem, GOAT* g) {

    const float DIM = 10.0f;

    if(!gem->exist) return;
    
    // If collision boxes overlay
    if(g->pos.x+8 >= gem->pos.x-DIM && g->pos.x-8 <= gem->pos.x+DIM
      && g->pos.y >= gem->pos.y-DIM && g->pos.y-16 <= gem->pos.y+DIM) {

          gem->exist = false;
          gem->deathTimer = DEATH_MAX;

          // Add gem to status
          status_add_coin();
    }
}