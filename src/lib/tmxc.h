/**
 * TMX file loader (header)
 * (NOTE: A lot of features missing, supports CSV only)
 * 
 * @author Jani Nykänen
 * @version 1.0.0
 */

#ifndef __TMXC__
#define __TMXC__

/// Map layer
typedef int* LAYER;

/** Tilemap type */
typedef struct
{
    // Map layers
    LAYER* layers;
    // Layer count
    int layerCount;
    // Map width
    int width;
    // Map height
    int height;
    // Tile width
    int tileW;
    // Tile height
    int tileH;
    // Pixel width
    int pwidth;
    // Pixel height;
    int pheight;
    // Tile count
    int tcount;
}
TILEMAP;

/**
 * Load a tilemap from a file
 * @param path File path
 * @return A new tilemap
 */
TILEMAP* load_tilemap(const char* path);


/**
 * Destroy a tilemap
 * @param t Tilemap to be destroyed
 */
void destroy_tilemap(TILEMAP* t);

#endif // __TMXC__