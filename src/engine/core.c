// GOAT
// Core (source)
// (c) 2018 Jani Nykänen

#include "core.h"

#include "error.h"
#include "scene.h"
#include "input.h"

#include <SDL2/SDL.h>

#include "../include/std.h"
#include "../include/renderer.h"
#include "../include/audio.h"

// Max scenes
#define MAX_SCENES 32

// Window size
static _POINT winSize;

// Is running
static bool isRunning;
// Is full screen
static bool isFullscreen;

// Window
static SDL_Window* window;
// Renderer
static SDL_Renderer* rend;

// (Timer) old ticks
static Uint32 oldTicks;
// New ticks
static Uint32 newTicks;
// Frame wait value
static Uint32 frameWait;

// Configuration
static CONFIG conf;

// Canvas position
static _POINT canvasPos;
// Canvas size
static _POINT canvasSize;
// Canvas
static FRAME* canvas;

// Scenes
static SCENE scenes[MAX_SCENES];
// Scene count
static int sceneCount;
// Current scene pointer
static int currentScene;
// Global scene pointer
static int globalScene = -1;

// Joystick
static SDL_Joystick* joy;


// Calculate canvas properties
static void calculate_canvas_prop(int winWidth, int winHeight) {

 
    // If aspect ratio is bigger or equal to the ratio of the canvas
    if((float)winWidth/(float)winHeight >= (float)canvas->width/ (float)canvas->height) {

        canvasSize.y = winHeight;
        canvasSize.x = (int) round( (float)winHeight / (float) canvas->height * canvas->width);

        canvasPos.x = winWidth/2 - canvasSize.x/2;
        canvasPos.y = 0;
    }
    else {

        canvasSize.x = winWidth;
        canvasSize.y = (int) round( (float)canvasSize.x / (float) canvas->width  * canvas->height);

        canvasPos.y = winHeight/2 - canvasSize.y/2;
        canvasPos.x = 0;
    }
}



// Show error
static void core_show_error(const char* msg) {

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", msg, window);
    printf("ERROR: %s\n", msg);
}


// Initialize SDL
static int core_init_SDL()
{   
    // Init
    if(SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0) {

        error_throw("Failed to initialize SDL!", NULL);
        return 1;
    }

    int windowWidth = conf.winSize.x;
    int windowHeight = conf.winSize.y;
    winSize = conf.winSize;

    // Create window
    window = SDL_CreateWindow(conf.caption, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				              windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
	if(window == NULL) {

		error_throw("Failed to create a window!", NULL);
        return 1;
	}

    isFullscreen = false;
    if(conf.fullscreen)
        core_toggle_fullscreen();

    // Create renderer
    int flag = conf.vsync ? (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) : SDL_RENDERER_ACCELERATED;
    rend = SDL_CreateRenderer(window,-1,flag);
    if(rend == NULL) {

        error_throw("Failed to create a renderer!", NULL);
        return 1;
    }

    // Clear
    SDL_SetRenderDrawColor(rend, 0,0,0, 255);
    SDL_RenderClear(rend);
    SDL_RenderPresent(rend);

    // Hide mouse cursor
    SDL_ShowCursor(0);

    // Calculate frame wait
    frameWait = 1000 / conf.frameRate;

    // Initialize audio
    if(init_audio() == 1) {

        return 1;
    }
    init_samples();
    init_music();

    // Get joystick, if any
    joy = SDL_JoystickOpen(0);
    if(joy == NULL) {

        printf("No joystick found. Ignoring.\n");
    }

    return 0;
}


// Initialize core
static int core_init() {

    // Initialize SDL content
    if(core_init_SDL() == 1) {

        return 1;
    }

    // Initialize other components
    graphics_init(rend);
    input_init();
    
    // Create canvas
    canvasPos = point(0,0);
    canvasSize = point(0,0);

    // Create frame
    canvas = frame_create(conf.canvasSize.x, conf.canvasSize.y);
    if(canvas == NULL) {

        return 1;
    }

    // Pass canvas to the renderer
    if(create_canvas_texture(canvas) == 1) {

        return 1;
    }
    bind_frame(canvas);
    update_canvas_texture();

    int i = 0;
    SCENE s;

    // Initialize scenes
    for(; i < sceneCount; ++ i) {

        s = scenes[i];
        if(s.fnInit != NULL) {

            if(s.fnInit() == 1) {

                return 1;
            }
        }
    }

    // Trigger resize event to set the canvas size
    calculate_canvas_prop(winSize.x, winSize.y);
    input_set_view_info(canvasPos, canvasSize);

    // Set the application running
    isRunning = true;

    return 0;
}


// Core events
static void core_events() {

    SDL_Event event;

    // Go through every event
    while (SDL_PollEvent(&event) != 0) {

        switch(event.type)
        {
        // Application quit
        case SDL_QUIT:

            isRunning = false;
            return;

        // Window event (resize etc)
        case SDL_WINDOWEVENT:
            // Resize
            if(event.window.windowID == SDL_GetWindowID(window) 
                && event.window.event == SDL_WINDOWEVENT_RESIZED) {
                
                   // Set window size
                   winSize.x = event.window.data1;
                   winSize.y = event.window.data2;

                   // Set canvas
                   calculate_canvas_prop(event.window.data1, event.window.data2);
                   // Pass canvas properties to input
                   input_set_view_info(canvasPos, canvasSize);
            }
            break;
        
        // Key down event
        case SDL_KEYDOWN:

            input_key_down((int)event.key.keysym.scancode);
            break;

        // Key up event
        case SDL_KEYUP:

            input_key_up((int)event.key.keysym.scancode);
            break;

        // Mouse move event
        case SDL_MOUSEMOTION:

            input_mouse_move(
                event.motion.x, event.motion.y
            );

            break;

        // Mouse button down event
        case SDL_MOUSEBUTTONDOWN:

            input_mouse_down(event.button.button);
            break;

        // Mouse button up event
        case SDL_MOUSEBUTTONUP:

            input_mouse_up(event.button.button);
            break;

         // Joy button down
        case SDL_JOYBUTTONDOWN:

            input_joy_down(event.jbutton.button);
            break;

        // Joy button up
        case SDL_JOYBUTTONUP:

            input_joy_up(event.jbutton.button);
            break;

        // Joy axis
        case SDL_JOYAXISMOTION:
        {
            int axis = 0;

            if(event.jaxis.axis == 0)
                axis = 0;

            else if(event.jaxis.axis == 1)
                axis = 1;
            else 
                break;

            float value = (float)event.jaxis.value / 32767.0f;

            input_joy_axis(axis,value);
            
            break;
        }

        // Joy hat
        case SDL_JOYHATMOTION:
        {
            int v = event.jhat.value;
            VEC2 stick = vec2(0.0f,0.0f);
            if(v == SDL_HAT_LEFTUP || v == SDL_HAT_LEFT || v == SDL_HAT_LEFTDOWN) {

                stick.x = -1.0f;
            }

            if(v == SDL_HAT_RIGHTUP || v == SDL_HAT_RIGHT || v == SDL_HAT_RIGHTDOWN){

                stick.x = 1.0f;
            }

            if(v == SDL_HAT_LEFTUP || v == SDL_HAT_UP || v == SDL_HAT_RIGHTUP) {

                stick.y = -1.0f;
            }

            if(v == SDL_HAT_LEFTDOWN || v == SDL_HAT_DOWN || v == SDL_HAT_RIGHTDOWN) {

                stick.y = 1.0f;
            }

            input_joy_axis(0,stick.x);
            input_joy_axis(1,stick.y);

            break;
        }

        default:
            break;
        }
    }
}


// Default key commands
static void default_key_commands() {

    // Quit
    if(input_get_key((int)SDL_SCANCODE_LCTRL) == STATE_DOWN &&
       input_get_key((int)SDL_SCANCODE_Q) == STATE_PRESSED) {

        isRunning = false;
    }

    // Full screen
    if(input_get_key((int)SDL_SCANCODE_F4) == STATE_PRESSED ||
       (input_get_key((int)SDL_SCANCODE_LALT) == STATE_DOWN &&
       input_get_key((int)SDL_SCANCODE_RETURN) == STATE_PRESSED) ) {

        core_toggle_fullscreen();
    }

}


// Update
static void core_update(Uint32 delta) {

    float tm = ((float)delta / 1000.0f) / (1.0f / 60.0f);

    // Default key commands (i.e quit & full screen)
    default_key_commands();

    // Update current scene
    SCENE s = scenes[currentScene];
    if(s.fnUpdate != NULL) {

        s.fnUpdate(tm);
    }

    // Update global scene
    if(globalScene != -1) {

        s = scenes[globalScene];
        if(s.fnUpdate != NULL) {

            s.fnUpdate(tm);
        }
    }

    // Update input
    input_update();
}


// Draw to canvas
static void core_draw_to_canvas() {

    // Draw current scene
    SCENE s = scenes[currentScene];
    if(s.fnDraw != NULL) {

        s.fnDraw();
    }

    // Draw global scene
    if(globalScene != -1) {

        s = scenes[globalScene];
        if(s.fnDraw != NULL) {

            s.fnDraw();
        }
    }

    update_canvas_texture();
}


// Draw
static void core_draw() {

    // Clear
    SDL_SetRenderDrawColor(rend, 0,0,0, 255);
    SDL_RenderClear(rend);
    
    // Draw canvas
    draw_canvas_texture(canvasPos, canvasSize);

}


// Destroy
static void core_destroy() {

    int i = 0;
    SCENE s;

    // Destroy scenes
    for(; i < sceneCount; ++ i) {

        s = scenes[i];
        if(s.fnDestroy != NULL) {
            
           s.fnDestroy();
        }
    }

    // Close joystick
    //if(joy != NULL)
    //   SDL_JoystickClose(joy);
        
    // Destroy content
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window);
}


// Loop, no vsync
static int core_loop_no_vsync() {

    Uint32 deltaTime = frameWait;

    // Loop
    while(isRunning) {

        // Set old time
        oldTicks = SDL_GetTicks();

        // Update frame
        core_events();
        core_update(deltaTime);
        core_draw_to_canvas();
        core_draw();

        // Render current frame
        SDL_RenderPresent(rend);

        // Check errors
        if(has_error()) {

            return 1;
        }

        // Set new time
        newTicks = SDL_GetTicks();

        // Wait
        int deltaMilliseconds = (newTicks - oldTicks);
        int restTime = (int) (frameWait-1) - (int)deltaMilliseconds;
        if (restTime > 0) 
            SDL_Delay((unsigned int) restTime);

        // Set delta time
        deltaTime = SDL_GetTicks() - oldTicks;
    }

    return 0;
}


// Loop, vsync enabled
static int core_loop_vsync() {

    oldTicks = SDL_GetTicks();

    int delta;
    int deltaSum =0;

    // Loop
    while(isRunning) {

        // Set new time
        newTicks = (int)SDL_GetTicks();

        // Calculate delta
        delta = newTicks - oldTicks;
        deltaSum += delta;

        // If enough time passed, update frame
        core_events();
        while(deltaSum >= frameWait) {

            core_update((Uint32)frameWait );
            core_draw_to_canvas();
            deltaSum -= frameWait;
        }

        // Set old time
        oldTicks = (int)SDL_GetTicks();

        // Render current frame
        core_draw();
        SDL_RenderPresent(rend);
    }

    return 0;
}


// Main loop
static int core_loop() {

    // Loop
    int (*fun)(void) = conf.vsync ? core_loop_vsync : core_loop_no_vsync;
    if(fun() == 1) {

        return 1;
    }

    // Destroy
    core_destroy();

    return 0;
}


// Add a scene
void core_add_scene(SCENE s) {

    // If we have more than 1 scene, make the first
    // scene the global scene
    if(sceneCount == 1) {

        globalScene = 0;
    }
    else if(sceneCount == 0) {

        globalScene = -1;
    }

    scenes[sceneCount] = s;
    currentScene = sceneCount ++;
}


// Run application
int core_run_application(const char* configPath) {

    // Read configuration (if exist)
    if(config_read(&conf, configPath) == 1) {

        // Set deftauls
        conf.winSize = point(640, 400);
        conf.canvasSize = point(320, 200);
        conf.frameRate = 30;
        snprintf(conf.caption, CAPTION_STRING_SIZE, "Game");
        conf.fullscreen = false;

        error_flush();
    }

    // Set error showing callback
    error_set_callback(core_show_error);

    // Initialize & loop
    if(core_init() == 1 
    || core_loop() == 1) {

        error_show();
        return -1;
    }

    return 0;
}


// Get window size
_POINT core_window_size() {

    return winSize;
}


// Toggle full screen
void core_toggle_fullscreen() {

    SDL_SetWindowFullscreen(window,!isFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
	    isFullscreen = !isFullscreen;
}


// Pass configuration
void core_pass_config(CONFIG c) {

    conf = c;
}


// Get configuration
CONFIG core_get_config() {

    return conf;
}


// Terminate
void core_terminate() {

    isRunning = false;
}


// Swap a scene
void core_swap_scene(const char* scene) {

    int i = 0;
    for(; i < sceneCount; ++ i) {

        if(strcmp(scenes[i].name,scene) == 0) {

            currentScene = i;
            if(scenes[i].fnSwap != NULL) {

                scenes[i].fnSwap();
            }
        }
    }
}
