// <Game title here>
// Audio (source)
// (c) 2018 Jani Nykänen

#include "audio.h"

#include <SDL2/SDL_mixer.h>

#include "../include/system.h"


// Initialize audio
int init_audio() {

    // Init formats
    int flags = MIX_INIT_OGG;
    int initted = Mix_Init(flags);
    if( (initted&flags) != flags)  {     

        error_throw("Failed to init audio!",NULL);
        return 1;
    }

    // Open audio
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512) == -1)  {     

        error_throw("Failed to open audio!",NULL);
        return 1;
    }

    return 0;
}
