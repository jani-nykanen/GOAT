// GOAT
// Camera (header)
// (c) 2018 Jani Nykänen

#ifndef __CAMERA__
#define __CAMERA__

#include "../include/renderer.h"

// Camera type
typedef struct {

    VEC2 pos;
}
CAMERA;

// Initialize global camera
void init_global_camera();

// Get global camera
CAMERA* get_global_camera();

// Use global camera
void use_global_camera();

// Update the camera
void move_camera(float speed, float tm);

#endif // __CAMERA__
