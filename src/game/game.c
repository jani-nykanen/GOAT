// <Insert project name here>
// Game scene (source)
// (c) 2018 Jani Nykänen

#include "game.h"

#include "stage.h"
#include "goat.h"
#include "camera.h"

#include "../global.h"
#include "../vpad.h"

#include "../include/system.h"

// Constants
static const float INITIAL_GLOBAL_SPEED = 0.5f;

// Bitmaps
static BITMAP* bmpFont;

// Global speed
static float globalSpeed;

// Game objects
static GOAT player;


// Initialize
static int game_init() {

    // Get assets
    ASSET_PACK* ass = global_get_asset_pack();
    bmpFont = (BITMAP*)assets_get(ass, "font");

    // Initialize components
    stage_init(ass);
    init_goat(ass);
    init_global_camera();

    // Reset
    game_reset();

    return 0;
}


// Update
static void game_update(float tm) {

    // Update components
    stage_update(globalSpeed, tm);
    goat_update(&player, tm);
    stage_goat_collision(&player);

    // Move camera
    move_camera(globalSpeed, tm);
}


// Draw
static void game_draw() {

    // Reset translation
    translate(0, 0);

    // Clear screen
    clear(0b10110111);

    // Draw components
    stage_draw();

    use_global_camera();
    goat_draw(&player);
}


// Destroy
static void game_destroy() {

}


// Change
static void game_on_change() {


}


// Reset
void game_reset() {

    // Set default values
    globalSpeed = INITIAL_GLOBAL_SPEED;
    get_global_camera()->pos = vec2(0, 0);

    // (Re)create game objects
    player = create_goat(vec2(128.0f,192.0f - 18.0f));
}


// Get scene
SCENE game_get_scene() {

    return scene_create(
        game_init,game_update,game_draw,game_destroy,game_on_change,
        "game");
}

