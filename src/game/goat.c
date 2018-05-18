// <Insert project name here>
// Goat (source)
// (c) 2018 Jani Nykänen

#include "goat.h"

#include "game.h"
#include "camera.h"

#include "../vpad.h"

#include "../include/std.h"

// Constants
static const float GOAT_SPEED = 0.15f;
static const float GOAT_TARGET = 1.5f;
static const float GOAT_GRAVITY = 0.125f;
static const float GOAT_GRAVITY_TARGET = 2.5f;
static const float GOAT_JUMP = -2.5f;
static const float GOAT_DASH_SPEED = 3.5f;
static const float DASH_TIMER_MAX = 20.0f;
static const float CLOUD_LIMIT = 30.0f;

// Bitmaps
static BITMAP* bmpGoat;


// Update cloud
static void update_cloud(CLOUD* g, float tm) {

    if(g->exist == false) return;

    g->timer += 1.0f * tm;

    if(g->timer >= CLOUD_LIMIT)
        g->exist = false;
}


// Generate clouds
static void generate_clouds(GOAT* g, float tm) {

    const float CLOUD_TIMER_LIMIT = 6.0f;
    const float DELTA = 0.25f;

    // Update clouds
    int i = 0;
    for(; i < CLOUD_COUNT; ++ i) {

        update_cloud(&g->clouds[i], tm);
    }

    // If not moving, don't generate
    if(g->canJump && hypotf(g->speed.x,g->speed.y) <= DELTA)
        return;

    // Generate
    g->cloudTimer += 1.0f * tm;
    if(g->cloudTimer >= CLOUD_TIMER_LIMIT) {

        g->cloudTimer -= CLOUD_TIMER_LIMIT;

        // Create cloud
        for(i=0; i < CLOUD_COUNT; ++ i) {

            if(g->clouds[i].exist == false) {

                g->clouds[i].pos = vec2(g->pos.x,g->pos.y);
                g->clouds[i].timer = 0;
                g->clouds[i].frame = g->spr.frame;
                g->clouds[i].row = g->spr.row;
                g->clouds[i].exist = true;
                g->clouds[i].flip = g->flip;

                return;
            }
        }
    }

}


// Control a goat
static void control_goat(GOAT* g) {

    const float DELTA = 0.01f;

    VEC2 stick = vpad_get_stick();
    if(fabsf(stick.x) <= DELTA)
        stick.x = 0.0f;

    // Horizontal movement
    g->target.x = stick.x * GOAT_TARGET;

    // Gravity
    g->target.y = GOAT_GRAVITY_TARGET;

    // Jump
    if(g->canJump && vpad_get_button(0) == STATE_PRESSED) {

        g->speed.y = GOAT_JUMP;
    }

    // Limit jump height by releasing the button
    if(!g->canJump && g->speed.y < 0.0f 
     && vpad_get_button(0) == STATE_RELEASED) {

        g->speed.y /= 1.75f;
    }

    // Dash
    g->dashing = (g->dashTimer < DASH_TIMER_MAX && !g->canJump && vpad_get_button(1) == STATE_DOWN);
    if(!g->dashing && g->dashTimer > 0.0f)  {

        g->dashTimer = DASH_TIMER_MAX;
    }
}


// Move axis
static void move_axis(float *pos, float* coord,  float* target, float speed, float tm) {

    if(*target > *coord) {

        *coord += speed * tm;
        if(*coord >= *target) {

            *coord = *target;
        }
    }
    else if(*target < *coord) {

        *coord -= speed * tm;
        if(*coord  <= *target) {

            *coord = *target;
        }
    }

    *pos += *coord *tm;
}


// Move a goat
static void move_goat(GOAT* g, float tm) {

    // Store old y position
    g->oldY = g->pos.y;

    // Dash
    if(g->dashing && g->dashTimer < DASH_TIMER_MAX) {

        g->target.y = 0.0f;
        g->speed.y = 0.0f;
        g->dashing = true;
        g->speed.x = (g->flip == 0 ? 1 : -1) * GOAT_DASH_SPEED;

        g->dashTimer += 1.0f * tm;
    }
    else {

        g->dashing = false;
    }

    // Move axes
    move_axis(&g->pos.x,&g->speed.x,&g->target.x, GOAT_SPEED, tm);
    move_axis(&g->pos.y,&g->speed.y,&g->target.y, GOAT_GRAVITY, tm);

    // Side teleport
    if(g->pos.x < 0.0f)
        g->pos.x += 256.0f;

    else if(g->pos.x > 256.0f)
        g->pos.x -= 256.0f;
}


// Animate a goat
static void animate_goat(GOAT* g, float tm) {

    const float DELTA = 0.01f;
    const float Y_DELTA = 0.5f;
    
    // Orientation
    if(!g->dashing && fabsf(g->target.x) > DELTA) {

        g->flip = g->target.x > 0.0f ? FLIP_NONE : FLIP_H;
    }

    // Dashing
    if(g->dashing) {

        if(!(g->spr.frame == 2 && g->spr.row == 2))
            spr_animate(&g->spr,2,0,2,6, tm);
    }
    // Running
    else if(g->canJump) {

        if(fabs(g->speed.x) > DELTA) {

            int speed = 6 - (int)roundf(fabsf(g->speed.x) / 0.5f);

            spr_animate(&g->spr,0,0,7,speed, tm);
        }
        else {

            g->spr.frame = 0;
            g->spr.row = 0;
        }
    }
    else {

        int frame = (int)floor( (g->speed.y+Y_DELTA/2.0f) / Y_DELTA);
        if(frame < -2) frame = -2;
        if(frame > 2) frame = 2;
        frame += 2;

        g->spr.frame = frame;
        g->spr.row = 1;
    }
}


// Draw a cloud sprite
static void draw_cloud_sprite(CLOUD* c, int x, int y) {

    int fade = 13- (int)floor(c->timer / CLOUD_LIMIT * 12);

    draw_bitmap_region_fading(bmpGoat,
        c->frame*32,c->row*32, 32,32, x,y,c->flip,fade, 255);
}


// Draw a cloud
static void draw_cloud(CLOUD* c) {

    if(!c->exist) return;

    int x = (int)roundf(c->pos.x-16);
    int y = (int)roundf(c->pos.y-28) +1;

    // "Original"
    draw_cloud_sprite(c, x, y);

    // "Outsider"
    if(c->pos.x < 16) {

        draw_cloud_sprite(c, x+256, y);
    }
    if(c->pos.x > 256- 16) {

        draw_cloud_sprite(c, x-256, y);
    }
}



// Draw a "single" goat
static void draw_single_goat(GOAT* g, int x, int y) {

    spr_draw(&g->spr,bmpGoat,x,y, g->flip);
}


// Initialize goats
void init_goat(ASSET_PACK* ass) {

    // Get assets
    bmpGoat = (BITMAP*)assets_get(ass, "goat");
}


// Create a new goat
GOAT create_goat(VEC2 p) {

    GOAT g;

    g.pos = p;
    g.oldY = p.y;
    g.speed = vec2(0,0);
    g.target = vec2(0,0);
    g.spr = create_sprite(32, 32);
    g.flip = FLIP_NONE;
    g.canJump = true;
    g.dashing = false;
    g.dashTimer = 0.0f;

    int i = 0;
    for(; i < CLOUD_COUNT; ++ i) {

        g.clouds[i].exist = false;
    }

    return g;
}


// Update goat
void goat_update(GOAT* g, float tm) {

    const float DELTA = -0.1f;

    control_goat(g);
    move_goat(g, tm);
    animate_goat(g, tm);
    generate_clouds(g, tm);

    g->canJump = false;

    // Death
    int camY = (int)get_global_camera()->pos.y;
    if(g->pos.y > camY+192+24 || (g->speed.y >= DELTA && g->pos.y <= camY) ) {

        game_reset();
    }
}


// Draw goat
void goat_draw(GOAT* g) {

    int x = (int)roundf(g->pos.x-16);
    int y = (int)roundf(g->pos.y-28) +1;

    // Draw clouds
    int i = 0;
    for(; i < CLOUD_COUNT; ++ i) {

        draw_cloud(&g->clouds[i]);
    }

    // "Original"
    draw_single_goat(g,x,y);

    // "Outsider"
    if(g->pos.x < g->spr.w/2) {

        draw_single_goat(g,x +256,y);
    }
    if(g->pos.x > 256- g->spr.w/2) {

        draw_single_goat(g,x -256,y);
    }
}


// Goat-to-floor collision
void goat_floor_collision(GOAT* g, float x, float y, float w) {

    const float WIDTH = 8;
    const float DELTA = 1.0f;

    if(g->pos.x >= x-WIDTH && g->pos.x < x+w+WIDTH) {

        if(g->speed.y > 0.0f && g->oldY < y+DELTA && g->pos.y > y-DELTA) {

            g->pos.y = y;
            g->speed.y = 0.0f;
            g->canJump = true;
            g->dashTimer = 0.0f;
        }
    }
}