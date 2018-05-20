// GOAT
// Virtual gamepad (header)
// (c) 2018 Jani Nykänen

#ifndef __VPAD__
#define __VPAD__

#include "engine/input.h"
#include "engine/vector.h"

// Initialize virtual gamepad
void vpad_init();

// Update vpad
void vpad_update();

// Get stick axis
VEC2 vpad_get_stick();

// Get stick delta
VEC2 vpad_get_delta();

// Add a new button
void vpad_add_button(unsigned char index, int scancode, int joybutton);

// Get virtual pad button state
int vpad_get_button(unsigned char index);

// Set stick position to zero
void vpad_flush_stick();

// Read configuration file
void vpad_read_config(const char* path);

#endif // __VPAD__
