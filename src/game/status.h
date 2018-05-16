// <Insert project name here>
// Status (header)
// (c) 2018 Jani Nykänen

#ifndef __STATUS__
#define __STATUS__

#include "../include/system.h"

// Initialize status
void init_status(ASSET_PACK* ass);

// Reset status
void reset_status();

// Update status
void status_update(float tm);

// Draw status
void status_draw();

// Reduce health
void status_reduce_health();

// Add a coin
void status_add_coin();

// Add score
void status_add_score();

#endif // __STATUS__
