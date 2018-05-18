// <Insert project name here>
// Stage (source)
// (c) 2018 Jani Nykänen

#include "stage.h"

#include "camera.h"
#include "game.h"
#include "status.h"

#include "../include/renderer.h"
#include "../include/std.h"

// Constants
#define PLATFORM_COUNT 5
#define TILE_COUNT 16
static const float CLOUD_SPEED = 0.5f;
static const float PLATFORM_INTERVAL = 64.0f;

// Bitmaps
static BITMAP* bmpSky;
static BITMAP* bmpClouds;
static BITMAP* bmpMountains;
static BITMAP* bmpPlatforms;

// Cloud position
static float cloudPos;

// Platform type
typedef struct {

    float y;
    int tiles[TILE_COUNT];
    int decorations[TILE_COUNT];
    int flip[TILE_COUNT]; // Decoration flips
    bool exist;
    bool scored;
}
PLATFORM;

// Platforms
static PLATFORM platforms[PLATFORM_COUNT];
// Platform timer
static float platTimer;


// Create the first platform
static void create_first_platform() {

    PLATFORM* p = &platforms[0];
    int i = 0;
    for(; i < TILE_COUNT; ++ i) {

        p->decorations[i] = -1;
        p->tiles[i] = i >= 4 && i < TILE_COUNT-4 ? 1 : 0;
    }
    p->y = 192 +48 - 64;
    p->exist = true;
    p->scored = true;
}


// Add gems to a platform
static void add_gems_to_platform(float y) {

    const int MAX_GEM = 4;

    int i = 0;
    float camY = floorf(get_global_camera()->pos.y);

    // How many
    int count = rand() % MAX_GEM;
    if(count > 0) {

        // Position
        int pos = rand() % (10 - count-1);
        for(i = 0; i < count; ++ i, ++ pos) {

            // Add a gem
            add_gem(8.0f + pos*24.0f + 12.0f,camY + y- 24.0f);
        }
    }
}


// Add a monster to a platform
static void add_monster_to_platform(int sx, int leftx, int len, int y, bool ground) {

    // Monster probability
    int monsterProb = 3;
    if(rand() % monsterProb != 0) return;

    bool groundType[] = {true, false};
    float yPositions[] = {0.0f, -14.0f};

    // Get random monster ID
    int id = rand() % 2;

    // If the enemy type is not suitable, get another one
    if(groundType[id] != ground) {

        // TODO: Other ids?
        id = ground ? 0 : 1;
    }

    int x = sx + rand() % (len-1);
    float ypos = floorf(get_global_camera()->pos.y) + (float)y + yPositions[id];
    float left = leftx*16.0f;
    float right = sx*16.0f + len*16.0f;

    add_monster(x*16.0f +8.0f, ypos, left, right, id);
}


// Create a new platform
static void create_platform() {

    const int MAX_HOLE_START = 4;
    const int MAX_HOLE_LENGTH = 5;
    const int MAX_GROUND_LENGTH = 6;
    const float Y_POS = 192 +48;

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
    platforms[p].scored = false;

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
    int notHoleStart = 0;

    for(i = 0; i < TILE_COUNT; ++ i) {
        
        if(-- count == 0) {

            created = false;
            isHole = !isHole;

            // Calculate hole/platform size
            if(isHole)
                count = rand() % MAX_HOLE_LENGTH + 2;
            
            else {

                if(bridgeBuilt != 1)
                    notHoleStart = i;
                
                count = rand() % MAX_GROUND_LENGTH + 1;
            }
            
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

            // Add monsters
            if(i < TILE_COUNT-2 && count >= 2)
                add_monster_to_platform(i, notHoleStart, count, Y_POS, !isHole || bridgeBuilt == 1);

            // Add decorations
            if(!isHole) {

                // Add big element
                if(count >= 4 && rand() % BIG_PROB == 0) {

                    decorationPos = i + rand() % (count - 4 +2);
                    if(decorationPos <= TILE_COUNT-4) {
    
                        platforms[p].decorations[decorationPos] = 6 + rand() % 3;
                        created = true;
                    }
                }
                else if(count >= 3) {
                    
                    decorationPos = i + rand() % (count-3 +2);
                    if(decorationPos <= TILE_COUNT-3) {

                        // Or tall element
                        if(rand() % TALL_PROB == 0) {

                            platforms[p].decorations[decorationPos] = 3 + rand() % 3;
                            created = true;
                        }
                        // Or low element
                        else if(rand() % LOW_PROB == 0) {

                            platforms[p].decorations[decorationPos] = 9 + rand() % 3;
                            created = true;
                        }

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
    platforms[p].y = Y_POS;
    platforms[p].exist = true;

    // Add gems
    add_gems_to_platform(Y_POS);

}


// Update all platforms
static void update_platforms(float globalSpeed, float tm) {

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

        draw_bitmap_region(bmpPlatforms,32+id*16,48,16,16,x*16,y-16, flip);
        break;

    // Tall
    case 3:
    case 4:
    case 5:

        draw_bitmap_region(bmpPlatforms,176 + (id-3)*32,0,32,48,x*16,y-48, flip);
        break;

    // Big
    case 6:
    case 7:
    case 8:

        draw_bitmap_region(bmpPlatforms,32 + (id-6)*48,16,48,32,x*16,y-32, flip);
        break;

    // Low
    case 9:
    case 10:
    case 11:

        draw_bitmap_region(bmpPlatforms,0,16 + (id-9)*16,32,16,x*16,y-16, flip);
        break;

    default:
        break;
    }
}


// Draw a single platform
static void draw_platform(PLATFORM* p) {

    int i = 0;
    int y = (int)floorf(p->y);
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


// Goat-to-platform collision
// TODO: Merge this and the following method
static void goat_platform_collision(GOAT* g, PLATFORM* p) {

    if(p->exist == false) return;

    float camY = get_global_camera()->pos.y;

    int i = 0;
    for(; i < TILE_COUNT; ++ i) {

        if(p->tiles[i] != 0) {

            goat_floor_collision(g, i*16,p->y + camY, 16);

            // If below the platform and not scored, score
            if(g->pos.y > p->y+camY+16 && !p->scored) {

                status_add_score();
                p->scored = true;
            }
        }
    }
}


// Gem-to-platform collision
static void gem_platform_collision(GEM* g, PLATFORM* p) {

    if(p->exist == false) return;

    float camY = get_global_camera()->pos.y;

    int i = 0;
    for(; i < TILE_COUNT; ++ i) {

        if(p->tiles[i] != 0) {

            gem_floor_collision(g, i*16,p->y + camY, 16);
        }
    }
}

// Initialize stage
int stage_init(ASSET_PACK* ass) {

    // Get assets
    bmpSky = (BITMAP*)assets_get(ass, "sky");
    bmpClouds = (BITMAP*)assets_get(ass, "clouds");
    bmpMountains = (BITMAP*)assets_get(ass, "mountains");
    bmpPlatforms = (BITMAP*)assets_get(ass, "platforms");

    // Set seed
    srand(time(NULL));

    // Reset
    stage_reset();

    return 0;
}


// Update stage
void stage_update(float globalSpeed, float tm) {

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
    update_platforms(globalSpeed, tm);
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


// Stage-to-goat collision
// TODO: Merge this and the following
void stage_goat_collision(GOAT* g) {

    int i = 0;
    for(; i < PLATFORM_COUNT; ++ i) {

        goat_platform_collision(g, &platforms[i]);
    }
}


// Stage-to-gem collision
void stage_gem_collision(GEM* g) {

    if(g->hasGravity == false) return;

    int i = 0;
    for(; i < PLATFORM_COUNT; ++ i) {

        gem_platform_collision(g, &platforms[i]);
    }
}


// Reset the stage
void stage_reset() {

    // Set default values
    cloudPos = 0.0f;
    platTimer = PLATFORM_INTERVAL;

    int i = 0;
    for(; i < PLATFORM_COUNT; ++ i) {

        platforms[i].exist = false;
    }

    // Create the first platform
    create_first_platform();
}