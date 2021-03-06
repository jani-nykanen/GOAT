// GOAT
// Game scene (source)
// (c) 2018 Jani Nykänen

#include "game.h"

#include "stage.h"
#include "goat.h"
#include "camera.h"
#include "status.h"
#include "gem.h"
#include "monster.h"
#include "pause.h"
#include "gameover.h"

#include "../global.h"
#include "../vpad.h"

#include "../include/system.h"
#include "../include/audio.h"

// Constants
#define GEM_COUNT 16
#define MONSTER_COUNT 16
static const float INITIAL_GLOBAL_SPEED = 0.5f;
static const float SPEED_UP_INTERVAL = 20.0f * 60.f;
static const float SPEED_UP = 0.1f;
static const int MAX_UP = 10;

// Global speed
static float globalSpeed;
static float speedCounter;
static int upCounter;

// Game objects
static GOAT player;
static GEM gems[GEM_COUNT];
static MONSTER monsters[MONSTER_COUNT];

// Is paused
static bool paused;

// Samples
static SAMPLE* sPause;


// Find the next gem
static GEM* find_next() {

    int i = 0;
    for(; i < GEM_COUNT; ++ i) {

        if(gems[i].exist == false && gems[i].deathTimer <= 0.0f) {

            return &gems[i];
        }
    }

    return &gems[0];
}


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
    sPause = (SAMPLE*)assets_get(ass, "pause");

    // Initialize components
    stage_init(ass);
    init_goat(ass);
    init_global_camera();
    init_status(ass);
    init_gems(ass);
    init_monsters(ass);
    init_game_over(ass);

    if(init_pause(ass) == 1)
        return 1;

    // Set default values
    paused = false;

    // Reset
    game_reset();

    return 0;
}


// Update
static void game_update(float tm) {

    int i = 0;
    int i2 = 0;

    // Do not update if fading
    if(is_fading()) return;

    // If paused
    if(pause_is_active()) {

        pause_update(tm);
        return;
    }

    // Check pause
    if(!status_is_game_over() 
     && (vpad_get_button(2) == STATE_PRESSED
     || vpad_get_button(3) == STATE_PRESSED) ) {

        play_sample(sPause, 0.70f);
        pause_active();
        return;
    }

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

    // Update game over
    gover_update(tm);
}


// Draw
static void game_draw() {

    const int SHAKE_COUNT = 7;

    // If pause active, draw it and ignore the rest
    if(pause_is_active()) {

        pause_draw();
        return;
    }

    int shakeX = 0;
    int shakeY = 0;
    int i = 0;

    // If player hurt, shake the screen
    if(player.hurtTimer > 0.0f) {

        shakeX = rand() % SHAKE_COUNT - (SHAKE_COUNT/2);
        shakeY = rand() % SHAKE_COUNT - (SHAKE_COUNT/2);
    }

    // Reset translation
    translate(shakeX, shakeY);

    // Clear screen
    clear(0b01000000);

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

    // Draw game over
    translate(0, 0);
    gover_draw();
}


// Destroy
static void game_destroy() {

    // pause_destroy();
}


// Change
static void game_on_change() {

    // game_reset();
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
    player = create_goat(vec2(128.0f,-4.0f));
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
    gover_reset();
}


// Get scene
SCENE game_get_scene() {

    return scene_create(
        game_init,game_update,game_draw,game_destroy,game_on_change,
        "game");
}


// Add a gem to the game world
void add_gem(float x, float y) {

    GEM* gem = find_next();
    *gem = create_gem(vec2(x, y));
}


// Add a gem with a gravity to the game world
void add_gem_with_gravity(float x, float y, float sx, float sy) {

    GEM* gem = find_next();
    *gem = create_gem_with_gravity(vec2(x, y), vec2(sx, sy));
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


// Get global speed
float get_global_speed() {

    return globalSpeed;
}
