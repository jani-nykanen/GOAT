// <Insert project name here>
// Pause menu (header)
// (c) 2018 Jani Nykänen

#ifndef __PAUSE__
#define __PAUSE__

#include "../include/system.h"
#include "../include/std.h"

// Initialize
int init_pause(ASSET_PACK* ass);

// Update
void pause_update(float tm);

// Draw
void pause_draw();

// Is the game paused
bool pause_is_active();

// Active pause
void pause_active();

#endif // __PAUSE__
