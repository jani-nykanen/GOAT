// <Insert project name here>
// Game scene (source)
// (c) 2018 Jani Nykänen

#include "game.h"

#include "stage.h"
#include "goat.h"
#include "camera.h"
#include "status.h"
#include "gem.h"
#include "monster.h"

#include "../global.h"
#include "../vpad.h"

#include "../include/system.h"

// Constants
#define GEM_COUNT 16
#define MONSTER_COUNT 16
static const float INITIAL_GLOBAL_SPEED = 0.5f;
static const float SPEED_UP_INTERVAL = 30.0f * 60.f;
static const float SPEED_UP = 0.1f;
static const int MAX_UP = 10;

// Bitmaps
static BITMAP* bmpFont;

// Global speed
static float globalSpeed;
static float speedCounter;
static int upCounter;

// Game objects
static GOAT player;
static GEM gems[GEM_COUNT];
static MONSTER monsters[MONSTER_COUNT];


// Update speed
static void update_speed(float tm) {

    if(upCounter >= MAX_UP) return;

    speedCounter += 1.0f * tm;
    if(speedCounter >= SPEED_UP_INTERVAL) {

        speedCounter -= SPEED_UP_INTERVAL;
        globalSpeed += SPEED_UP;

        ++ upCounter;
    }
}


// Initialize
static int game_init() {

    // Get assets
    ASSET_PACK* ass = global_get_asset_pack();
    bmpFont = (BITMAP*)assets_get(ass, "font");

    // Initialize components
    stage_init(ass);
    init_goat(ass);
    init_global_camera();
    init_status(ass);
    init_gems(ass);
    init_monsters(ass);

    // Reset
    game_reset();

    return 0;
}


// Update
static void game_update(float tm) {

    int i = 0;
    int i2 = 0;

    // Update stage
    stage_update(globalSpeed, tm);

    // Update player
    goat_update(&player, tm);
    stage_goat_collision(&player);

    // Update gems
    for(i = 0; i < GEM_COUNT; ++ i) {

        gem_update(&gems[i], tm);
        gem_goat_collision(&gems[i], &player);
        stage_gem_collision(&gems[i]);
    }

    // Update monsters
    for(i = 0; i < MONSTER_COUNT; ++ i) {

        monster_update(&monsters[i], tm);
        monster_goat_collision(&monsters[i], &player);

        for(i2 = 0; i2 < MONSTER_COUNT; ++ i2) {

            if(i2 == i) continue;
            monster_to_monster_collision(&monsters[i], &monsters[i2]);
        }
    }

    // Update status
    status_update(tm);

    // Move camera
    move_camera(globalSpeed, tm);

    // Update global speed
    update_speed(tm);
}


// Draw
static void game_draw() {

    int i = 0;

    // Reset translation
    translate(0, 0);

    // Clear screen
    clear(0b10110111);

    // Draw stage
    stage_draw();

    // Draw game objects
    use_global_camera();
    for(i = 0; i < GEM_COUNT; ++ i) {

        gem_draw(&gems[i]);
    }
    for(i = 0; i < MONSTER_COUNT; ++ i) {

        monster_draw(&monsters[i]);
    }
    goat_draw(&player);

    // Draw status
    translate(0, 0);
    status_draw();
}


// Destroy
static void game_destroy() {

}


// Change
static void game_on_change() {


}


// Reset
void game_reset() {

    int i = 0;

    // Set default values
    globalSpeed = INITIAL_GLOBAL_SPEED;
    get_global_camera()->pos = vec2(0, 0);
    upCounter = 0;
    speedCounter = 0;

    // (Re)create game objects
    player = create_goat(vec2(128.0f,192.0f - 18.0f));
    for(i = 0; i < GEM_COUNT; ++ i) {

        gems[i].exist = false;
        gems[i].deathTimer = -1.0f;
    }
    for(i = 0; i < MONSTER_COUNT; ++ i) {

        monsters[i].exist = false;
        monsters[i].deathTimer = -1.0f;
    }

    // Reset components
    stage_reset();
    reset_status();
}


// Get scene
SCENE game_get_scene() {

    return scene_create(
        game_init,game_update,game_draw,game_destroy,game_on_change,
        "game");
}


// Add a gem to the game world
// TODO: A method for "find first gem"
void add_gem(float x, float y) {

    // Find the first gem that does not exist
    int i = 0;
    for(; i < GEM_COUNT; ++ i) {

        if(gems[i].exist == false && gems[i].deathTimer <= 0.0f) {

            gems[i] = create_gem(vec2(x, y));
            return;
        }
    }
}


// Add a gem with a gravity to the game world
void add_gem_with_gravity(float x, float y, float sx, float sy) {

    // Find the first gem that does not exist
    int i = 0;
    for(; i < GEM_COUNT; ++ i) {

        if(gems[i].exist == false && gems[i].deathTimer <= 0.0f) {

            gems[i] = create_gem_with_gravity(vec2(x, y), vec2(sx, sy));
            return;
        }
    }
}


// Add a monster to the game world
void add_monster(float x, float y, float left, float right, int id) {

    // Find the first monster that does not exist
    int i = 0;
    for(; i < MONSTER_COUNT; ++ i) {

        if(monsters[i].exist == false && monsters[i].dying == false) {

            monsters[i] = create_monster(vec2(x, y), left, right, id);
            return;
        }
    }
}


// Get the amount of speed ups
int get_speed_up_count() {

    return upCounter;
}
