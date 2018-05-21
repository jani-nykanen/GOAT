// GOAT
// Leaderboard menu scene (header)
// (c) 2018 Jani Nykänen

#ifndef __LB_MENU__
#define __LB_MENU__

#include "../engine/scene.h"

// Menu types
enum {

    LB_MENU_SHOW = 0,
    LB_MENU_SUBMIT = 1,
    LB_MENU_SENDING = 2,
};

// Set menu type
void set_lb_menu_type(int type);

// Get leaderboard menu scene
SCENE lb_menu_get_scene();

#endif // __LB_MENU__
