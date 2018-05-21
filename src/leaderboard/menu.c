// GOAT
// Leaderboard menu scene (header)
// (c) 2018 Jani Nykänen

#include "menu.h"

#include "../global.h"
#include "../vpad.h"

#include "../include/std.h"
#include "../include/system.h"
#include "../include/renderer.h"


// Initialize
static int lb_menu_init() {

    return 0;
}


// Update
static void lb_menu_update(float tm) {

}


// Draw
static void lb_menu_draw() {

}


// Swap
static void lb_menu_on_change() {

}


// Set menu type
void set_lb_menu_type(int type) {

}


// Get leaderboard menu scene
SCENE get_lb_menu_scene() {

    return scene_create(
        lb_menu_init,lb_menu_update,lb_menu_draw,NULL,lb_menu_on_change,
        "lbmenu");
}
