// <Game title here>
// Music (header)
// (c) 2018 Jani Nykänen

#ifndef __MUSIC__
#define __MUSIC__

#include <SDL2/SDL_mixer.h>

#include <stdbool.h>

// Music
typedef struct {

    Mix_Music* data;
}
MUSIC;

// Init music
void init_music();

// Load music
MUSIC* load_music(const char* path);

// Fade in music
void fade_in_music(MUSIC* mus, float vol, int loops, int time);

// Play music
void play_music(MUSIC* mus, float vol, int loops);

// Destroy music
void destroy_music(MUSIC* m);

// Stop music
void stop_music();

// Fade out music
void fade_out_music(int ms);

// Set global music volume
void enable_music(bool state);

// Is music enabled
bool music_enabled();

// Set global music volume
void set_global_music_volume(int vol);

// Return global music volume
int get_global_music_volume();

#endif // __MUSIC__
