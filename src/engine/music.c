// <Game title here>
// Music (source)
// (c) 2018 Jani Nykänen

#include "music.h"

#include "../include/system.h"
#include "../include/std.h"


// Global music volume
static int globalMusicVol;
// Is music playing
static bool playing;
// Music enabled
static bool musicEnabled;
// Old volume
static float oldVol;


// Init music
void init_music() {

    globalMusicVol = 100;
    playing = false;
    musicEnabled = true;
    oldVol = 1.0f;
}   


// Load music
MUSIC* load_music(const char* path) {

    MUSIC* m = (MUSIC*)malloc(sizeof(MUSIC));
    if(m == NULL) {     

        error_mem_alloc();
        return NULL;
    }

    m->data = Mix_LoadMUS(path);
    if(m->data == NULL) {     

        error_throw("Failed to load a music file in ",path);
        return NULL;
    }
    return m;
}

// Fade in music
void fade_in_music(MUSIC* mus, float vol, int loops, int time) {

    if(mus == NULL || !musicEnabled) return;

    oldVol = vol;

    float mvol = (float)globalMusicVol / 100.0f;

    int v = (int)(128 * vol * mvol);
    if(v > 128) v = 128;
    if(v < 0) v = 0;

    Mix_HaltMusic();
    Mix_VolumeMusic(v);
    Mix_FadeInMusic(mus->data, loops,time);

    playing = true;
}


// Play music
void play_music(MUSIC* mus, float vol, int loops) {

    fade_in_music(mus, vol, loops, 0);
}


// Destroy music
void destroy_music(MUSIC* m) {

    if(m == NULL) return;

    Mix_FreeMusic(m->data);
    free(m);
}


// Stop music
void stop_music() {

    if(!musicEnabled) return;

    Mix_FadeOutMusic(1000);
}


// Fade out
void fade_out_music(int ms) {

    Mix_FadeOutMusic(ms);
}


// Enable music
void enable_music(bool state) {

    if(!state) {     

        Mix_PauseMusic();
        globalMusicVol = 0;
    }
    else {     

        Mix_ResumeMusic();
        globalMusicVol = 100;
    }
    musicEnabled = state;
    
}


// Set global music volume
void set_global_music_volume(int vol) {

    float mvol = (float)vol / 100.0f;
    int v = (int)(128 * oldVol * mvol);

    Mix_VolumeMusic(v);

    globalMusicVol = vol;
}


// Return global music volume
int get_global_music_volume() {

    return globalMusicVol;
}