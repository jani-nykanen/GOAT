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

// Add a gem with a gravity to the game world
void add_gem_with_gravity(float x, float y, float sx, float sy);

// Add a monster to the game world
void add_monster(float x, float y, float left, float right, int id);

// Get the amount of speed ups
int get_speed_up_count();

#endif // __GAME__

