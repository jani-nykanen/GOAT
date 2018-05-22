// GOAT
// Title screen scene (source)
// (c) 2018 Jani Nykänen

#include "title.h"

#include "../global.h"
#include "../vpad.h"
#include "../cursor.h"

#include "../game/stage.h"
#include "../game/game.h"
#include "../leaderboard/menu.h"

#include "../include/std.h"
#include "../include/system.h"
#include "../include/renderer.h"
#include "../include/audio.h"

// Menu items
static const char* MENU_ITEMS[] = {
    "Play game", "Leaderboard", "Full screen", "Audio: On", "Quit"
};

// Constants
static const float LOGO_AMPLITUDE = 12.0f;
static const float ENTER_AMPLITUDE = 6.0f;
static const float LOGO_WAVE_SPEED = 0.0625f;
static const float GO_AWAY_MAX = 60.0f;
static const int ELEMENT_COUNT = 5;

// Bitmaps
static BITMAP* bmpFont;
static BITMAP* bmpFont2;
static BITMAP* bmpFontBig;
static BITMAP* bmpLogo;
static BITMAP* bmpCursor;

// Samples
static SAMPLE* sAccept;
static SAMPLE* sReject;
static SAMPLE* sPause;
static SAMPLE* sSelect;

// Logo wave timer
static float logoWave;
// Go away timer
static float goAway;
// Is going away
static bool goingAway;
// Phase
static int phase;
// Enter pressed
static bool enterPressed;

// Cursor
static CURSOR cursor;


// Draw logo
static void draw_logo(int dx, int dy) {

    const float PERIOD = M_PI / 4.0f;

    int w = bmpLogo->width / 4;

    int i = 0;
    int y;
    for(; i < 4; ++ i) {

        y = dy + (int)floorf(sinf(logoWave + PERIOD*i) * LOGO_AMPLITUDE);

        draw_bitmap_region(bmpLogo, i*w,0,w,bmpLogo->height, dx + w*i, y, 0);
    }

}


// Draw menu text
static void draw_menu_text(int x, int y, int yoff) {

    bool audioState = music_enabled() && samples_enabled();

    const char* text;
    const char* AUDIO_TEXT_OFF = "Audio: Off";

    int i = 0;
    for(; i < ELEMENT_COUNT; ++ i) {

        text = MENU_ITEMS[i];
        if(i == 3 && !audioState)
            text = AUDIO_TEXT_OFF;

        draw_text( (cursor.moving || (cursor.pos != i)) ? bmpFont : bmpFont2, 
            text,x,y +yoff*i,-7,0,false);
    }
}


// Draw "Press enter" text
static void draw_press_enter(int dx, int dy, int xoff) {

    const float PERIOD = M_PI / 10;
    const char* TEXT = "PRESS ENTER";
    char str[1];

    int i = 0;
    int y;
    for(; i < 11; ++ i) {

        if(i == 5) continue;

        y = dy + (int)floorf(sinf(logoWave + PERIOD*i) * ENTER_AMPLITUDE);

        str[0] = TEXT[i];
        draw_text(bmpFontBig,(const char*)str, dx + i* (-xoff), y, xoff,0,false);
    }
}


// Menu action
static void menu_action() {

    bool audioState = music_enabled() && samples_enabled();
    bool playSample = true;

    switch(cursor.pos) {

    // Play game
    case 0:

        goingAway = true;
        goAway = 0.0f;
        break;

    // Leaderboard
    case 1:
        
        set_lb_menu_type(LB_MENU_FETCHING);
        core_swap_scene("lbmenu");
        break;
        
    // Full screen
    case 2:
        core_toggle_fullscreen();
        break;

    // Audio
    case 3: 
        if(audioState) {

            enable_music(false);
            enable_samples(false);

            playSample = false;
        }
        else {

            enable_music(true);
            enable_samples(true);
        }

        break;

    // Quit
    case 4:
        fade(1, 2.0f, core_terminate);
        break;

    default:
        break;

    }

    if(playSample) {

        play_sample(sAccept, 0.80f);
    }

}


// Initialize
static int ts_init() {

    // Get assets
    ASSET_PACK* ass = global_get_asset_pack();
    
    bmpFont = (BITMAP*)assets_get(ass, "font");
    bmpFont2 = (BITMAP*)assets_get(ass, "font2"); 
    bmpLogo = (BITMAP*)assets_get(ass, "logo"); 
    bmpFontBig = (BITMAP*)assets_get(ass, "fontBig");
    bmpCursor = (BITMAP*)assets_get(ass, "cursor");

    sAccept = (SAMPLE*)assets_get(ass, "accept");
    sReject = (SAMPLE*)assets_get(ass, "reject");
    sPause = (SAMPLE*)assets_get(ass, "pause");
    sSelect = (SAMPLE*)assets_get(ass, "select");

    // Set defaults
    logoWave = 0.0f;
    goingAway = false;
    goAway = 0.0f;
    enterPressed = false;
    phase = 0;

    // Create cursor
    cursor = create_cursor(ELEMENT_COUNT);

    return 0;
}


// Update
static void ts_update(float tm) {

    // Update background
    stage_update_bg_only(tm);

    // Update waves
    logoWave += LOGO_WAVE_SPEED * tm;

    if(is_fading()) return;

    if(goingAway) {

        goAway += 1.0f * tm;
        if(goAway >= GO_AWAY_MAX) {

            game_reset();
            core_swap_scene("game");
        }
    }
    else {

        if(phase == 1) {

            cursor_update(&cursor, tm);
        }

        // Accept button down
        if(vpad_get_button(0) == STATE_PRESSED
        || vpad_get_button(2) == STATE_PRESSED) {

            if(phase == 0) {

                ++ phase;
                play_sample(sPause, 0.80f);
                enterPressed = true;
            }
            else {

                menu_action();
            }
        }

        // Quit
        if(vpad_get_button(3) == STATE_PRESSED) {

            fade(1,2.0f, core_terminate);
            play_sample(sReject, 0.70f);
        }
    }
    
}


// Draw
static void ts_draw() {

    const int YOFF = 12;
    const int MENU_Y = 192-76;
    const int MENU_X = 64;

    translate(0, 0);

    // Draw stage background
    stage_draw();

    // Darken
    int dark = 4;
    if(goingAway) {

        dark = 4 - (int)floorf(goAway / GO_AWAY_MAX * 4.0f);
    }

    if(dark > 0)
        darken(dark);

    // Draw logo
    int y = 12;
    if(goingAway) {

        y = 12 - (int)floorf(goAway / GO_AWAY_MAX * 128.0f);
    }
    draw_logo(128-100, y);

    // Translate
    if(goingAway) {

        y = (int)floorf(goAway / GO_AWAY_MAX * 96.0f);
        translate(0, y);
    }

    if(phase == 0) {

        // Draw "Press enter"
        draw_press_enter(35,192-56, -15);
    }
    else {

        // Draw menu
        draw_menu_text(MENU_X,MENU_Y, YOFF);

        // Draw cursor
        cursor_draw(&cursor, MENU_X, MENU_Y, YOFF);
    }

    // Draw copyright
    draw_text(bmpFont, "# 2018 Jani Nyk~nen",128,192-14, -7,0, true);
}


// Swap
static void ts_on_change() {

    stage_reset();
    goingAway = false;
    goAway = 0.0f;

    if(enterPressed) {

        phase = 1;
    }
    else {

        cursor = create_cursor(ELEMENT_COUNT);
    }
}


// Get leaderboard menu scene
SCENE ts_get_scene() {

    return scene_create(
        ts_init,ts_update,ts_draw,NULL,ts_on_change,
        "title");
}
