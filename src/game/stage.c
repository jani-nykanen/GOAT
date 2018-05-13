// <Insert project name here>
// Stage (source)
// (c) 2018 Jani Nykänen

#include "stage.h"

#include "../include/renderer.h"
#include "../include/std.h"

// Constants
#define PLATFORM_COUNT 5
#define TILE_COUNT 16
static const float CLOUD_SPEED = 0.5f;
static const float INITIAL_GLOBAL_SPEED = 1.0f;
static const float PLATFORM_INTERVAL = 64.0f;

// Bitmaps
static BITMAP* bmpSky;
static BITMAP* bmpClouds;
static BITMAP* bmpMountains;
static BITMAP* bmpPlatforms;

// Cloud position
static float cloudPos;
static float globalSpeed;

// Platform type
typedef struct {

    float y;
    int tiles[TILE_COUNT];
    int decorations[TILE_COUNT];
    int flip[TILE_COUNT]; // Decoration flips
    bool exist;
}
PLATFORM;

// Platforms
static PLATFORM platforms[PLATFORM_COUNT];
// Platform timer
static float platTimer;


// Create a new platform
static void create_platform() {

    const int MAX_HOLE_START = 4;
    const int MAX_HOLE_LENGTH = 5;
    const int MAX_GROUND_LENGTH = 6;

    const int BRIDGE_PROB = 4;
    const int BIG_PROB = 3;
    const int TALL_PROB = 3;
    const int LOW_PROB = 3;
    const int SMALL_PROB = 2;

    // Find the first platform that is not in the game
    int i = 0;
    for(; i < PLATFORM_COUNT; ++ i) {

        if(platforms[i].exist == false) {

            break;
        }
    }
    int p = i;

    // Clear decorations (and set flippings)
    for(i = 0; i < TILE_COUNT; ++ i) {

        platforms[p].decorations[i] = -1;
        platforms[p].flip[i] = rand() % 2;
    }

    // Add holes
    int count = (rand() % 2 == 0) ? 1 : (rand() % MAX_HOLE_START +2);
    bool isHole =false;
    int bridgeBuilt =0;
    int decorationPos = 0;
    bool created = false;

    for(i = 0; i < TILE_COUNT; ++ i) {
        
        if(-- count == 0) {

            created = false;
            isHole = !isHole;

            // Calculate hole/platform size
            if(isHole)
                count = rand() % MAX_HOLE_LENGTH + 2;
            else
                count = rand() % MAX_GROUND_LENGTH + 1;
            
            // If bridge ending, set bridge state to 2 
            // so bridges are built no longer
            if(bridgeBuilt == 1)
                ++ bridgeBuilt;

            else {

                if(count < MAX_GROUND_LENGTH && 
                    isHole && bridgeBuilt == 0 && rand() % BRIDGE_PROB == 0) {

                    bridgeBuilt = 1;
                }
            }

            // Add decorations
            if(!isHole) {

                // Add big element
                if(count >= 4 && rand() % BIG_PROB == 0) {

                    decorationPos = i + rand() % (count - 4 +2);
                    if(decorationPos > TILE_COUNT-1)
                        decorationPos = TILE_COUNT-1;

                    platforms[p].decorations[decorationPos] = 3 + 2 + rand() % 2;
                    created = true;
                }
                else if(count >= 3) {
                    
                    decorationPos = i + rand() % (count-3 +2);
                    if(decorationPos > TILE_COUNT-1)
                            decorationPos = TILE_COUNT-1;

                    // Or tall element
                    if(rand() % TALL_PROB == 0) {

                        platforms[p].decorations[decorationPos] = 3 + rand() % 2;
                        created = true;
                    }
                    // Or low element
                    else if(rand() % LOW_PROB == 0) {

                        platforms[p].decorations[decorationPos] = 3 + 2 + 2 + rand() % 2;
                        created = true;
                    }

                }
                // Or small element
                if(!created && rand() % SMALL_PROB == 0) {

                    decorationPos = i + rand() % (count);
                    if(decorationPos > TILE_COUNT-1)
                            decorationPos = TILE_COUNT-1;

                    platforms[p].decorations[decorationPos] = rand() % 3;
                }
            }
        }

        if(bridgeBuilt != 1)
            platforms[p].tiles[i] = !isHole ? 1 : 0;

        else
            platforms[p].tiles[i] = 2;
    }
    platforms[p].y = 192 +48;
    platforms[p].exist = true;

}


// Update all platforms
static void update_platforms(float tm) {

    int i = 0;
    PLATFORM* p;
    for(; i < PLATFORM_COUNT; ++ i) {

        if(platforms[i].exist == false)
            continue;

        // Update platform
        p = &platforms[i];
        p->y -= globalSpeed * tm;
        if(p->y < -16.0f) {

            p->exist = false;
        }
    }
}


// Draw a decoration
static void draw_decoration(int id, int x, int y, int flip) {

    switch(id) {

    // Small
    case 0:
    case 1:
    case 2:

        draw_bitmap_region(bmpPlatforms,256-16,id*16,16,16,x*16,y-16, flip);
        break;

    // Tall
    case 3:
    case 4:

        draw_bitmap_region(bmpPlatforms,160 + (id-3)*32,0,32,48,x*16,y-48, flip);
        break;

    // Big
    case 5:
    case 6:

        draw_bitmap_region(bmpPlatforms,32 + (id-5)*48,16,48,32,x*16,y-32, flip);
        break;

    // Low
    case 7:
    case 8:

        draw_bitmap_region(bmpPlatforms,0,16 + (id-7)*16,32,16,x*16,y-16, flip);
        break;

    default:
        break;
    }
}


// Draw a single platform
static void draw_platform(PLATFORM* p) {

    int i = 0;
    int y = (int)floor(p->y);
    bool left, right;
    int tile =0;


    // Draw bridge
    for(; i < TILE_COUNT; ++ i) {

        if(p->tiles[i] == 2) {

            // Are adjacent tiles bridges, too
            left = i > 0 && p->tiles[i-1] != 2;
            right = i < TILE_COUNT-1 && p->tiles[i+1] != 2;

            // Draw "stripe"
            draw_bitmap_region(bmpPlatforms,8*16,0,16,16, i*16, y-16, 0);

            if(left)
                draw_bitmap_region(bmpPlatforms,7*16,0,16,16, i*16 -16, y-16, 0);

            if(right)
                draw_bitmap_region(bmpPlatforms,9*16,0,16,16, i*16 +16, y-16, 0); 

            draw_bitmap_region(bmpPlatforms,6*16,0,16,16, i*16, y, 0);
        }
    }

    // Draw decorations
    for(i = 0; i < TILE_COUNT; ++ i) {

        if(p->decorations[i] != -1)
            draw_decoration(p->decorations[i], i, p->y, p->flip[i]);
    }

    // Draw ground
    for(i = 0; i < TILE_COUNT; ++ i) {

        if(p->tiles[i] == 1) {
            
            // Are adjacent tiles ground, too 
            left = i > 0 && p->tiles[i-1] != 1;
            right = i < TILE_COUNT-1 && p->tiles[i+1] != 1;

            if(!left && !right)
                tile = 3;

            else if(left && right)
                tile = 0;

            else if(left)
                tile = 2;

            else if(right)
                tile = 4;

            draw_bitmap_region(bmpPlatforms,tile*16,0,16,16, i*16, y, 0);

            // Draw grass to left & right
            if(left)
                draw_bitmap_region(bmpPlatforms,1*16,0,16,16, (i-1)*16, y, 0);

            if(right)
                draw_bitmap_region(bmpPlatforms,5*16,0,16,16, (i+1)*16, y, 0);
        }
    }
}


// Draw all platforms
static void draw_platforms() {

    int i = 0;
    for(; i < PLATFORM_COUNT; ++ i) {

        if(platforms[i].exist == false)
            continue;

        // Draw platform
        draw_platform(&platforms[i]);
    }
}


// Initialize stage
int stage_init(ASSET_PACK* ass) {

    // Get assets
    bmpSky = (BITMAP*)assets_get(ass, "sky");
    bmpClouds = (BITMAP*)assets_get(ass, "clouds");
    bmpMountains = (BITMAP*)assets_get(ass, "mountains");
    bmpPlatforms = (BITMAP*)assets_get(ass, "platforms");

    // Set default values
    cloudPos = 0.0f;
    globalSpeed = INITIAL_GLOBAL_SPEED;
    platTimer = PLATFORM_INTERVAL;

    int i = 0;
    for(; i < PLATFORM_COUNT; ++ i) {

        platforms[i].exist = false;
    }

    // Set seed
    srand(time(NULL));

    return 0;
}


// Update stage
void stage_update(float tm) {

    // Make the clouds move again
    cloudPos -= CLOUD_SPEED * tm;
    if(cloudPos <= -(float)bmpClouds->width) {

        cloudPos += (int)bmpClouds->width;
    }

    // Create platforms
    platTimer += 1.0f * tm * globalSpeed;
    if(platTimer >= PLATFORM_INTERVAL) {

        platTimer -= PLATFORM_INTERVAL;
        create_platform();
    }

    // Update platforms
    update_platforms(tm);
}


// Player collision
void stage_pl_collision(void* pl, float tm) {

}


// Draw stage
void stage_draw() {

    int i = 0;
    int cpos = (int)round(cloudPos);

    // Sky
    draw_bitmap_fast(bmpSky, 0, 0);

    // Clouds
    for(; i < 2; ++ i) {

        draw_bitmap(bmpClouds,cpos + i*bmpClouds->width, 80, 0);
    }

    // Mountains
    draw_bitmap(bmpMountains,0,192 -96, 0);

    // Draw platforms
    draw_platforms();
}
