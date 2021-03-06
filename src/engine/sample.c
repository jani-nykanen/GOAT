// <Game title here>
// Audio sample (source)
// (c) 2018 Jani Nykänen

#include "sample.h"

#include "../include/system.h"
#include "../include/std.h"

// Global volume
static int globalSoundVol;
// Samples enabled
static bool samplesEnabled;


// Init audio
void init_samples() {

    // Set default values
    globalSoundVol = 100;
    samplesEnabled = true;
}


// Set global volume
void set_global_sample_volume(int vol) {

    globalSoundVol = vol;
}


// Load a sound
SAMPLE* load_sample(const char* path) {

    // Allocate memory
    SAMPLE * s = (SAMPLE*)malloc(sizeof(s));
    if(s == NULL) {     

        error_mem_alloc();
        return NULL;
    }

    // Load WAV
    s->chunk =Mix_LoadWAV(path);
    if(!s->chunk) {     
    
        error_throw("Failed to load a sound file in ",path);
        free(s);
        return NULL;
    }

    // Set default values
    s->channel = 0;
    s->played = false;

    return s;
}


// Play sound
void play_sample(SAMPLE* s, float vol) {

    if(s == NULL || !samplesEnabled) return;

    float svol = (float)globalSoundVol / 100.0f;

    if(!s->played) {     

        // Get channel, halt, set volume, play again
        s->channel = Mix_PlayChannel(-1,s->chunk,0);
        Mix_HaltChannel(s->channel);

        Mix_Volume(s->channel,(int)(MIX_MAX_VOLUME * vol * svol));
        Mix_PlayChannel(-1,s->chunk,0);
        
        s->played = true;
    }
    else {     

        Mix_HaltChannel(s->channel);
        Mix_Volume(s->channel,(int)(MIX_MAX_VOLUME * vol * svol));
        Mix_PlayChannel(s->channel,s->chunk,0);
    }
}


// Stop all samples
void stop_all_samples() {

    Mix_HaltChannel(-1);
}


// Destroy sound & free memory
void destroy_sample(SAMPLE* s) {

    if(s == NULL) return;

    Mix_FreeChunk(s->chunk);
    free(s);
}


// Enable/disable samples
void enable_samples(bool state) {

    samplesEnabled = state;
}


// Get global sample volume
int get_global_sample_volume() {

    return globalSoundVol;
}


// Are samples enabled
bool samples_enabled() {

    return samplesEnabled;
}
