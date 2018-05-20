// GOAT
// Virtual gamepad (source)
// (c) 2018 Jani Nykänen

#include "vpad.h"

#include "lib/readword.h"

#include "include/std.h"

// Max button count
#define BUTTON_MAX 256


// Button object
typedef struct
{
    int scancode;
    int joybutton;
}
BUTTON;

// "Analogue" stick
static VEC2 stick;
// Old stick state
static VEC2 oldStick;
// Delta
static VEC2 delta;
// Buttons
static BUTTON buttons[BUTTON_MAX];


// Initialize virtual gamepad
void vpad_init()
{
    stick.x = 0.0f;
    stick.y = 0.0f;
}


// Update vpad
void vpad_update()
{
    oldStick = stick;

    stick.x = 0.0f;
    stick.y = 0.0f;

    // Arrow keys
    if(input_get_key((int)SDL_SCANCODE_LEFT) == STATE_DOWN) {
    
        stick.x = -1.0f;
    }
    else if(input_get_key((int)SDL_SCANCODE_RIGHT) == STATE_DOWN) {
    
        stick.x = 1.0f;
    }

    if(input_get_key((int)SDL_SCANCODE_UP) == STATE_DOWN) {
    
        stick.y = -1.0f;
    }
    else if(input_get_key((int)SDL_SCANCODE_DOWN) == STATE_DOWN) {
    
        stick.y = 1.0f;
    }

    // Joystick
    VEC2 jstick = input_get_joy_axis();
    if(hypot(jstick.x,jstick.y) > 0.1f) {
    
        stick.x = jstick.x;
        stick.y = jstick.y;   
    }

    // Calculate delta
    delta.x = stick.x - oldStick.x;
    delta.y = stick.y - oldStick.y;
}


// Get stick axis
VEC2 vpad_get_stick() {

    return stick;
}


// Get stick delta
VEC2 vpad_get_delta() {

    return delta;
}


// Add a new button
void vpad_add_button(unsigned char index, int scancode, int joybutton) {

    buttons[index] = (BUTTON){scancode, joybutton};
}


// Get virtual pad button state
int vpad_get_button(unsigned char index) {

    int ret = input_get_key(buttons[index].scancode);
    if(ret == STATE_UP) {
    
        ret = input_get_joy_state(buttons[index].joybutton);
    }
    
    return ret;
}


// Flush stick
void vpad_flush_stick() {

    stick.x = 0.0f;
    stick.y = 0.0f;
}

// Read configuration file
void vpad_read_config(const char* path) {

    // NOTE: The application will NOT terminate even
    // if the file parsing fails

    // Create a word reader
    WORDREADER* wr = wr_create('#');
    if(wr == NULL) {

        return;
    }

    // Open for reading
    if(wr_open(wr, path) == 1) {

        printf("Failed to open a key configuration file in %s. Ignoring.\n", path);
        wr_destroy_reader(wr);
    }

    // Read 
    int value =0;
    int count =0;

    int id, key, button;

    // Read until stop
    while(wr_read_next(wr)) {

        value = (int)strtol(wr->buffer, NULL, 10);

        if(count == 0) {

            id = value;
        }
        else if(count == 1) {

            key = value;
        }
        else if(count == 2) {

            button = value;

            // Add a button
            vpad_add_button((unsigned char)id, key, button);
        }
        ++ count;
        count %= 3;
    }

    // Close & destroy
    wr_close(wr);
    wr_destroy_reader(wr);

}
