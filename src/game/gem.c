// GOAT
// Gem (source)
// (c) 2018 Jani Nykänen

#include "gem.h"

#include "status.h"
#include "camera.h"

#include "../include/std.h"
#include "../include/audio.h"

// Constants
static const float AMPLITUDE = 4.0f;
static const float WAVE_SPEED = 0.05f;
static const float DEATH_MAX = 30.0f;
static const float GEM_GRAVITY = 0.1f;
static const float MAX_GRAVITY = 2.5f;
static const float GEM_SLOW_X = 0.025f;
static const float WAIT_TIME = 20.0f;
static const int HEART_PROB_MAX = 16;

// Bitmaps
static _BITMAP* bmpGem;

// Samples
static SAMPLE* sGem;
static SAMPLE* sHeal;

// Heart probability
static int heartProb;


// Initialize gems
void init_gems(ASSET_PACK* ass) {

    // Get Bitmaps
    bmpGem = (_BITMAP*)assets_get(ass, "gem");

    sGem = (SAMPLE*)assets_get(ass, "sGem");
    sHeal = (SAMPLE*)assets_get(ass, "heal");

    // Set heart probability to max
    heartProb = HEART_PROB_MAX;
}


// Create gem, base
static GEM create_gem_base(VEC2 pos) {

    GEM g;
    g.pos = pos;
    g.spr = create_sprite(24, 24);
    g.waveTimer = (float)(rand() % 1000) / 1000.0f * 2 * M_PI;
    g.deathTimer = 0.0f;
    g.exist = true;
    g.hasGravity = false;
    g.waitTimer = -1.0f;

    return g;
}


// Create a gem
GEM create_gem(VEC2 pos) {
    
    GEM g = create_gem_base(pos);

    if(heartProb > 2)
        -- heartProb;
    g.isHeart = (rand() % heartProb == 0);
    
    if(g.isHeart)
        heartProb = HEART_PROB_MAX;

    return g;
}


// Create a gem with gravity
GEM create_gem_with_gravity(VEC2 pos, VEC2 speed) {

    GEM g = create_gem_base(pos);
    g.hasGravity = true;
    g.speed = speed;
    g.waveTimer = 0.0f;
    g.oldY = 0.0f;
    g.waitTimer = WAIT_TIME;
    g.isHeart = false;

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

    // Update wait timer
    if(gem->waitTimer > 0.0f)
        gem->waitTimer -= 1.0f * tm;

    // Store old y coordinate
    gem->oldY = gem->pos.y;

    float camY = get_global_camera()->pos.y;

    // Wave
    if(!gem->hasGravity) {
        gem->waveTimer += WAVE_SPEED * tm;

    } 
    else {

        // Update gravity
        gem->speed.y += GEM_GRAVITY * tm;
        if(gem->speed.y > MAX_GRAVITY)
            gem->speed.y = MAX_GRAVITY;

        // Update horizontal speed
        if(gem->speed.x > 0.0f) {

            gem->speed.x -= GEM_SLOW_X * tm;
            if(gem->speed.x < 0.0f)
                gem->speed.x = 0.0f;
        }
        else if(gem->speed.x < 0.0f) {

            gem->speed.x += GEM_SLOW_X * tm;
            if(gem->speed.x > 0.0f)
                gem->speed.x = 0.0f;
        }

        // Movement
        gem->pos.x += gem->speed.x *tm;
        gem->pos.y += gem->speed.y *tm;

        // If outside the screen (horizontal)
        if(gem->pos.x > 256.0f)
            gem->pos.x -= 256.0f;

        else if(gem->pos.x < 0.0f)
            gem->pos.x += 256.0f;

        // Bottom
        if(gem->pos.y > camY+192+16.0f) {

            gem->exist = false;
            gem->deathTimer = -1.0f;
        }
    }

    // Animate
    if(gem->isHeart)
        spr_animate(&gem->spr,1,0,5, 5, tm);
    
    else
        spr_animate(&gem->spr,0,0,4, 5, tm);

    // If outside the screen, "kill"
    if(gem->pos.y < camY - 12.0f - AMPLITUDE) {

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
            draw__BITMAP_region_fading(bmpGem,0,gem->isHeart ? 24 : 0,24,24,x,y, FLIP_NONE, fade, get_alpha());
        }   
        return;
    }

    // Draw
    spr_draw(&gem->spr,bmpGem, x,y, FLIP_NONE);

    // "Off-screen"
    if(gem->hasGravity) {

        if(gem->pos.x < 16.0f)
            spr_draw(&gem->spr,bmpGem, x +256.0f,y, FLIP_NONE);

        else if(gem->pos.x > 256.0f-16.0f)
            spr_draw(&gem->spr,bmpGem, x - 256.0f,y, FLIP_NONE);
    }
}


// Gem-to-goat collision
void gem_goat_collision(GEM* gem, GOAT* g) {

    const float DIM = 10.0f;

    if(!gem->exist || gem->waitTimer > 0.0f) return;
    
    // If collision boxes overlay
    if(g->pos.x+8 >= gem->pos.x-DIM && g->pos.x-8 <= gem->pos.x+DIM
      && g->pos.y >= gem->pos.y-DIM && g->pos.y-16 <= gem->pos.y+DIM) {

          gem->exist = false;
          gem->deathTimer = DEATH_MAX;

          // Add gem to status
          (gem->isHeart ? status_add_health : status_add_coin) ();

          play_sample(gem->isHeart ? sHeal : sGem, 0.65f);
    }
}


// Gem-to-floor collision
void gem_floor_collision(GEM* g, float x, float y, float w) {

    const float WIDTH = 4;
    const float DELTA = 1.0f;

    if(g->pos.x >= x-WIDTH && g->pos.x < x+w+WIDTH) {

        if(g->speed.y > 0.0f && g->oldY+8.0f < y+DELTA && g->pos.y+8.0f > y-DELTA) {

            g->pos.y = y-8.0f;
            g->speed.y *= -0.90f;
        }
    }
}
