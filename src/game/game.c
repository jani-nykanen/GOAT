// <Insert project name here>
// Game scene (source)
// (c) 2018 Jani Nykänen

#include "game.h"

#include "stage.h"
#include "goat.h"
#include "camera.h"
#include "status.h"
#include "gem.h"

#include "../global.h"
#include "../vpad.h"

#include "../include/system.h"

// Constants
#define GEM_COUNT 16
static const float INITIAL_GLOBAL_SPEED = 0.5f;

// Bitmaps
static BITMAP* bmpFont;

// Global speed
static float globalSpeed;

// Game objects
static GOAT player;
static GEM gems[GEM_COUNT];


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

    // Reset
    game_reset();

    return 0;
}


// Update
static void game_update(float tm) {

    int i = 0;

    // Update stage
    stage_update(globalSpeed, tm);

    // Update game objects
    goat_update(&player, tm);
    stage_goat_collision(&player);
    for(i = 0; i < GEM_COUNT; ++ i) {

        gem_update(&gems[i], tm);
        gem_goat_collision(&gems[i], &player);
    }

    // Update status
    status_update(tm);

    // Move camera
    move_camera(globalSpeed, tm);
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

    // (Re)create game objects
    player = create_goat(vec2(128.0f,192.0f - 18.0f));
    for(i = 0; i < GEM_COUNT; ++ i) {

        gems[i].exist = false;
        gems[i].deathTimer = -1.0f;
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
