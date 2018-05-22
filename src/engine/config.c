// GOAT
// Configuration (source)
// (c) 2018 Jani Nykänen

#include "config.h"

#include "../lib/readword.h"

#include "error.h"

// Constants
#define BUFFER_SIZE 128


// Read a config file
int config_read(CONFIG* c, const char* path) {

    // Create a word reader
    WORDREADER* wr = wr_create('#');
    if(wr == NULL) {

        error_mem_alloc();
        return 1;
    }

    // Open file
    if(wr_open(wr, path) == 1) {

        error_throw("Word reader error: ", (const char*)wr->error);
    }


    // Key
    static char key[BUFFER_SIZE];
    // Value
    static char* value;

    // Read words
    while(wr_read_next(wr)) {

        if(strcmp(wr->buffer,"=") == 0) {

            wr_read_next(wr);
            if(key[0] != '$') continue;

            value = wr->buffer;

            if(strcmp(key,"$window_caption") == 0) {

                strcpy(c->caption,value);
            }
            else if(strcmp(key,"$window_width") == 0) {

                c->winSize.x = (int)strtol(value,NULL,10);
            }
            else if(strcmp(key,"$window_height") == 0) {

                c->winSize.y = (int)strtol(value,NULL,10);
            }
            else if(strcmp(key,"$full_screen") == 0) {

                c->fullscreen = (int)strtol(value,NULL,10);
            }
            else if(strcmp(key,"$vsync") == 0) {

                c->vsync = (int)strtol(value,NULL,10);
            }
            else if(strcmp(key,"$canvas_width") == 0) {

                c->canvasSize.x = (int)strtol(value,NULL,10);
            }
            else if(strcmp(key,"$canvas_height") == 0) {

                c->canvasSize.y = (int)strtol(value,NULL,10);
            }
            else if(strcmp(key,"$frame_rate") == 0) {

                c->frameRate = (int)strtol(value,NULL,10);
            }
            else if(strcmp(key,"$asset_path") == 0) {

                strcpy(c->assetPath,value);
            }
            else if(strcmp(key,"$keyconf_path") == 0) {

                strcpy(c->keyconfPath, value);
            }
            else if(strcmp(key,"$music_volume") == 0) {

                c->musicVol = (int)strtol(value,NULL,10);
            }
            else if(strcmp(key,"$sample_volume") == 0) {

                c->sampleVol = (int)strtol(value,NULL,10);
            }

        }
        // Store the current value to the key
        else {

            snprintf(key, BUFFER_SIZE, "%s", wr->buffer);
        }
    }

    // Close reader & free memory
    wr_close(wr);
    wr_destroy_reader(wr);

    return 0;
}