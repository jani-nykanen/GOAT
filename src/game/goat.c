// <Insert project name here>
// Goat (source)
// (c) 2018 Jani Nykänen

#include "goat.h"

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

// Bitmaps
static BITMAP* bmpGoat;


// Control a goat
static void control_goat(GOAT* g) {

    VEC2 stick = vpad_get_stick();

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
    g->dashing = (!g->canJump && vpad_get_button(1) == STATE_DOWN);
    if(g->dashing && (vpad_get_button(1) == STATE_UP || vpad_get_button(1) == STATE_RELEASED))  {

        g->dashTimer = DASH_TIMER_MAX;
        g->dashing = false;
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

        g->spr.frame = 5;
        g->spr.row = 1;
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

    return g;
}


// Update goat
void goat_update(GOAT* g, float tm) {

    control_goat(g);
    move_goat(g, tm);
    animate_goat(g, tm);

    g->canJump = false;
}


// Draw goat
void goat_draw(GOAT* g) {

    int x = (int)roundf(g->pos.x-16);
    int y = (int)roundf(g->pos.y-28) +1;

    // "Original"
    spr_draw(&g->spr,bmpGoat,x,y, g->flip);

    // "Outsider"
    if(g->pos.x < g->spr.w/2) {

        spr_draw(&g->spr,bmpGoat,x + 256,y, g->flip);
    }
    if(g->pos.x > 256- g->spr.w/2) {

        spr_draw(&g->spr,bmpGoat,x - 256,y, g->flip);
    }
}


// Goat-to-floor collision
void goat_floor_collision(GOAT* g, float x, float y, float w) {

    const float WIDTH = 8;
    const float DELTA = 0.1f;

    if(g->pos.x >= x-WIDTH && g->pos.x < x+w+WIDTH) {

        if(g->speed.y > 0.0f && g->oldY < y+DELTA && g->pos.y > y-DELTA) {

            g->pos.y = y;
            g->speed.y = 0.0f;
            g->canJump = true;
            g->dashTimer = 0.0f;
        }
    }
}