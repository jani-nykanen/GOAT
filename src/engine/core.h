// <Insert project name here>
// Core (header)
// (c) 2018 Jani Nykänen

#ifndef __CORE__
#define __CORE__

#include "scene.h"
#include "vector.h"
#include "config.h"

// Add a scene
void core_add_scene(SCENE s);

// Run application
int core_run_application(const char* configPath);

// Get window size
POINT core_window_size();

// Toggle full screen
void core_toggle_fullscreen();

// Get configuration
CONFIG core_get_config();

// Terminate
void core_terminate();

// Swap a scene
void core_swap_scene(const char* scene);

#endif // __CORE__