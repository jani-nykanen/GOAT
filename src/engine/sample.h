// <Game title here>
// Audio sample (header)
// (c) 2018 Jani Nykänen

#ifndef __SAMPLE__
#define __SAMPLE__

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <stdbool.h>

// Sample type
typedef struct {

    Mix_Chunk* chunk;
    int channel;
    bool played;
}
SAMPLE;

// Init sample
void init_samples();

// Set global sample volume
void set_global_sample_volume(int vol);

// Load a sample
SAMPLE* load_sample(const char* path);

// Play a sample
void play_sample(SAMPLE* s, float vol);

// Stop all samples
void stop_all_samples();

// Destroy a sample
void destroy_sample(SAMPLE* s);

// Enable/disable samples
void enable_samples(bool state);

// Get global sample volume
int get_global_sample_volume();

#endif // __SAMPLE__
