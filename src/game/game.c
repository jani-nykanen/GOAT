// <Insert project name here>
// Game scene (source)
// (c) 2018 Jani Nykänen

#include "game.h"

#include "stage.h"

#include "../global.h"
#include "../vpad.h"

#include "../include/system.h"

// Bitmaps
static BITMAP* bmpFont;


// Initialize
static int game_init() {

    // Get assets
    ASSET_PACK* ass = global_get_asset_pack();
    bmpFont = (BITMAP*)assets_get(ass, "font");

    // Initialize components
    stage_init(ass);

    // Reset
    game_reset();

    return 0;
}


// Update
static void game_update(float tm) {

    // Update components
    stage_update(tm);
}


// Draw
static void game_draw() {

    // Clear screen
    clear(0b10110111);

    // Draw components
    stage_draw();
}


// Destroy
static void game_destroy() {

}


// Change
static void game_on_change() {


}


// Reset
void game_reset() {


}


// Get scene
SCENE game_get_scene() {

    return scene_create(
        game_init,game_update,game_draw,game_destroy,game_on_change,
        "game");
}

