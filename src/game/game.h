// <Insert project name here>
// Game scene (header)
// (c) 2018 Jani Nykänen

#ifndef __GAME__
#define __GAME__

#include "../engine/scene.h"

#include "../include/renderer.h"

// Reset game
void game_reset();

// Get game scene
SCENE game_get_scene();

// Add a gem to the game world
void add_gem(float x, float y);

#endif // __GAME__

