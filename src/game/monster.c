// <Insert project name here>
// Monster (source)
// (c) 2018 Jani Nykänen

#include "monster.h"

#include "../include/std.h"

// Constants
static const float DEATH_MAX = 30.0f;
static const float WALKER_SPEED = 0.5f;

// Bitmaps
static BITMAP* bmpMonsters;


// "Set monster"
static void set_monster(MONSTER* m) {

    switch(m->id) {

    case 0:
        m->direction = rand() % 2 == 0 ? 1 : -1;
        m->speed.x = WALKER_SPEED * m->direction;
        
        break;

    default:
        break;
    }

}


// Move monster
static void move_monster(MONSTER* m, float tm) {

    // Move axes
    m->pos.x += m->speed.x * tm;
    m->pos.y += m->speed.y * tm;

    // Limit collisions
    if(m->speed.x < 0.0f && m->pos.x-8.0f < m->leftLimit) {

        m->speed.x *= -1;
        m->pos.x = m->leftLimit +8.0f;
    }
    else if(m->speed.x > 0.0f && m->pos.x+8.0f > m->rightLimit) {

        m->speed.x *= -1;
        m->pos.x = m->rightLimit -8.0f;
    }

}


// Animate
static void animate_monster(MONSTER* m, float tm) {

    switch(m->id) {

    case 0:
        spr_animate(&m->spr, m->id,0,3, 6, tm);
        m->flip = m->speed.x > 0.0f ? FLIP_H : FLIP_NONE;
        break;

    default:
        break;
    }
}


// Initialize monsters
void init_monsters(ASSET_PACK* ass) {

    // Get assets
    bmpMonsters = (BITMAP*)assets_get(ass, "monsters");
}


// Create a monster
MONSTER create_monster(VEC2 pos, float left, float right, int id) {

    MONSTER m;
    m.pos = pos;
    m.id = id;
    m.deathTimer = 0.0f;
    m.speed = vec2(0, 0);
    m.spr = create_sprite(32, 32);
    m.exist = true;
    m.flip = FLIP_NONE;
    m.leftLimit = left;
    m.rightLimit = right;
    if(m.rightLimit > 256)
        m.rightLimit = 256;

    // Set id-depending values
    set_monster(&m);

    return m;
}


// Update a monster
void monster_update(MONSTER* m, float tm) {

    if(m->exist == false) {

        // Update death timer
        if(m->deathTimer > 0.0f) {

            m->deathTimer -= 1.0f * tm;
        }
        return;
    }

    // Move
    move_monster(m, tm);

    // Animate
    animate_monster(m, tm);
}


// Draw a monster
void monster_draw(MONSTER* m) {

    // Rendering position
    int x = (int)floorf(m->pos.x-16.0f);
    int y = (int)floorf(m->pos.y-28.0f) +1;

    if(!m->exist) {

        // Fading
        if(m->deathTimer > 0.0f) {

            int fade = 1+ (int)roundf(m->deathTimer / DEATH_MAX * 8.0f);
            draw_bitmap_region_fading(bmpMonsters,0,m->id*32,
                32,32,x,y, m->flip, fade, get_alpha());
        }   
        return;
    }

    // Draw
    spr_draw(&m->spr,bmpMonsters, x,y, m->flip);

    // Limits
    // fill_rect((int)m->leftLimit, y,2,16, 0b11100000);
    // fill_rect((int)m->rightLimit, y,2,16, 0b11100000);
}


// Monster-to-goat collision
void monster_goat_collision(MONSTER* m, GOAT* g) {

    const float GOAT_JUMP_BACK = -2.0f;

    if(!m->exist) return;

    // If in the same horizontal area
    if(g->pos.x+8.0f >= m->pos.x-8.0f && g->pos.x-8.0f <= m->pos.x+8.0f) {

        // If the goat is jumping over the monster
        if(g->speed.y > 0.0f && g->pos.y > m->pos.y-26.0f && g->pos.y < m->pos.y-16.0f) {

            m->exist = false;
            m->deathTimer = DEATH_MAX;

            g->speed.y = GOAT_JUMP_BACK;
        }

    }
}
