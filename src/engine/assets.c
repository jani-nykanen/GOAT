// GOAT
// Assets (source)
// (c) 2018 Jani Nykänen

#include "assets.h"

#include "bitmap.h"
#include "error.h"

#include "../lib/tmxc.h"
#include "../lib/readword.h"

#include "../include/std.h"
#include "../include/audio.h"

// Constants
#define BUFFER_SIZE 128

// Asset type enum
enum {

    T_BITMAP = 0,
    T_TILEMAP = 1,
    T_SAMPLE = 2,
    T_MUSIC = 3,
};


// Get asset type
static int get_asset_type(const char* value) {

    if(strcmp(value,"bitmap")  == 0) {

        return T_BITMAP;
    }
    else if(strcmp(value,"tilemap") == 0) {

        return T_TILEMAP;
    }
    else if(strcmp(value,"sample") == 0) {

        return T_SAMPLE;
    }
    else if(strcmp(value,"music") == 0) {

        return T_MUSIC;
    }
    
    return -1;
}


// Load an asset
static void* load_asset(const char* path, int type) {

    switch(type) {

    case T_BITMAP:
        return bitmap_load(path);

    case T_TILEMAP:
        return load_tilemap(path);

    case T_MUSIC:
        return load_music(path);

    case T_SAMPLE:
        return load_sample(path);

    default:
        return NULL;
    }
}


// Load an asset pack
ASSET_PACK* load_asset_pack(const char* fpath) {

    // Create a word reader
    WORDREADER* wr = wr_create('#');
    if(wr == NULL) {

        error_mem_alloc();
        return NULL;
    }

    // Open for reading
    if(wr_open(wr, fpath) == 1) {

        error_throw("Word reader error: ", (const char*)wr->error);
        return NULL;
    }

    // Allocate memory
    ASSET_PACK* p = malloc(sizeof(ASSET_PACK));
    if(p == NULL) {

        error_mem_alloc();
        return NULL;
    }
    p->assetCount = 0;

    
    int assetType;       

    int count = 0;
    
    char* value;

    char param[BUFFER_SIZE];
    char name[BUFFER_SIZE];
    char filePath[BUFFER_SIZE];
    char path[NAME_STRING_SIZE];

    // Go through words
    while(wr_read_next(wr)) {

        if(strcmp(wr->buffer, "=") == 0) {
             
            // param = pw_get_word(w, i-1);
            wr_read_next(wr);
            value = wr->buffer;

            // Asset type
            if(strcmp(param,"$type") == 0) {

                assetType = get_asset_type(value);
            }
            // Asset path
            else if(strcmp(param,"$path") == 0) {

                snprintf(filePath, BUFFER_SIZE, "%s", value);
                
            }
        }
        else if(wr->buffer[0] != '$') {

            if(count == 0) {

                snprintf(name, BUFFER_SIZE, "%s", wr->buffer);
            }
            else {

                snprintf(path, NAME_STRING_SIZE, "%s%s", filePath, wr->buffer);
                p->objects[p->assetCount] = load_asset(path,assetType);
                if(p->objects[p->assetCount] == NULL) {

                    return NULL;
                }
                strcpy(p->names[p->assetCount].data, name);
                ++ p->assetCount;
            }

            count = !count;
        }
        
        // Copy the current word to the parameter string
        snprintf(param, BUFFER_SIZE, "%s", wr->buffer);
    }

    // Close word reader & delete it
    wr_close(wr);
    wr_destroy_reader(wr);

    return p;
}


// Get asset pack
ANY assets_get(ASSET_PACK* p, const char* name) {

    int i = 0;
    // Go through names and try to find an asset with the given name
    for(; i < p->assetCount; ++ i) {

        if(strcmp(name,p->names[i].data) == 0) {

            return p->objects[i];
        }
    }

    return NULL;
}


// Destroy an asset pack
void assets_destroy(ASSET_PACK* p) {

    int i = 0;
    ANY obj;
    for(; i < p->assetCount; ++ i) {   

        obj = p->objects[i];
        switch(p->types[i])
        {
        case T_BITMAP:
            bitmap_destroy((BITMAP*)obj);
            break;
        case T_TILEMAP:
            destroy_tilemap((TILEMAP*)obj);
            break;
        case T_SAMPLE:
            destroy_sample((SAMPLE*)obj);
            break;
        case T_MUSIC:
            destroy_music((MUSIC*)obj);
            break;

        default:
            break;
        }
    }
}
