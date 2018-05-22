// GOAT
// Main (source)
// (c) 2018 Jani Nykänen

#define SDL_MAIN_HANDLED

#include "engine/core.h"

// Scenes
#include "game/game.h"
#include "leaderboard/menu.h"
#include "title/title.h"
#include "title/intro.h"
#include "global.h"


// Main
int main(int argc, char** argv) {
    
    // Add scenes
    core_add_scene(global_get_scene());
    core_add_scene(lb_menu_get_scene());
    core_add_scene(game_get_scene());
    core_add_scene(ts_get_scene());
    core_add_scene(intro_get_scene());
    
    return core_run_application("properties.conf");
}

