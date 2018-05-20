// GOAT
// Camera (header)
// (c) 2018 Jani Nykänen

#include "camera.h"

// Global camera
static CAMERA globalCamera;


// Initialize global camera
void init_global_camera() {

    // Create the global camerea
    globalCamera = (CAMERA){ vec2(0, 0) };
}


// Get global camera
CAMERA* get_global_camera(){

    return &globalCamera;
}


// Use global camera
void use_global_camera() {

    translate(-(int)round(globalCamera.pos.x), -(int)round(globalCamera.pos.y));
}


// Move the camera
void move_camera(float speed, float tm) {

    globalCamera.pos.y += speed *tm;
}
