/**
 * Scene (source)
 * 
 * @author Jani Nykänen
 * @version 1.0.0
 */

#include "scene.h"

#include "stdio.h"
#include "stdlib.h"


// Create a new scene
SCENE scene_create(int (*fnInit) (void), void (*fnUpdate) (float),
                   void (*fnDraw) (void), void (*fnDestroy) (void), 
                   void (*fnSwap), 
                   const char* name) {

    SCENE s = (SCENE){fnInit,fnUpdate,fnDraw,fnDestroy,fnSwap};
    snprintf(s.name,SCENE_NAME_LENGTH,"%s",name);

    return s;
}