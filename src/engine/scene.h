// GOAT
// Scene (header)
// (c) 2018 Jani Nykänen

#ifndef __SCENE__
#define __SCENE__

// Scene name length
#define SCENE_NAME_LENGTH 64

// Scene type
typedef struct {

    int (*fnInit) (void);
    void (*fnUpdate) (float);
    void (*fnDraw) (void);
    void (*fnDestroy) (void);
    void (*fnSwap) (void);

    char name[SCENE_NAME_LENGTH];
}
SCENE;

// Create a new scene
SCENE scene_create(int (*fnInit) (void), void (*fnUpdate) (float),
                   void (*fnDraw) (void), void (*fnDestroy) (void), 
                   void (*fnSwap), 
                   const char* name);

#endif // __SCENE__