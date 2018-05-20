// GOAT
// Assets (header)
// (c) 2018 Jani Nykänen

#ifndef __ASSETS__
#define __ASSETS__

// Asset name size
#define NAME_STRING_SIZE 128

// Initial buffer size
#define INITIAL_BUFFER_SIZE 512

// Literally any type
typedef void* ANY;

// Asset name
typedef struct {

    char data[NAME_STRING_SIZE];
}
NAME;

// Asset pack type
typedef struct {

    int* types;
    ANY objects[INITIAL_BUFFER_SIZE];
    NAME names[INITIAL_BUFFER_SIZE];
    unsigned int assetCount;
}
ASSET_PACK;

// Load an asset pack
ASSET_PACK* load_asset_pack(const char* path);

// Get asset pack
ANY assets_get(ASSET_PACK* p, const char* name);

// Destroy an asset pack
void assets_destroy(ASSET_PACK* p);

#endif // __ASSETS__
