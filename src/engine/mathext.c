// GOAT
// Math extensions (source)
// (c) 2018 Jani Nykänen

#include "mathext.h"


// Minimum (out of 3)
int min_3(int a, int b, int c) {

    return (a <= b && a <= c) ? a 
        : ( (b <= a && b <=c) ? b : c);
}


// Maximum (out of 3)
int max_3(int a, int b, int c) {

    return (a >= b && a >= c) ? a 
        : ( (b >= a && b >=c) ? b : c);
}


// Minimum (out of 3), float
float min_3f(float a, float b, float c) {

    return (a <= b && a <= c) ? a 
        : ( (b <= a && b <=c) ? b : c);
}


// Maximum (out of 3), float
float max_3f(float a, float b, float c) {

    return (a >= b && a >= c) ? a 
        : ( (b >= a && b >=c) ? b : c);
}


// Minimum (out of 2)
int min_2(int a, int b) {

    return (a <= b ? a : b);
}


// Maximum (out of 2)
int max_2(int a, int b) {

    return (a >= b ? a : b);
}



// Minimum (out of 2), float
float min_2f(float a, float b) {

    return (a <= b ? a : b);
}


// Maximum (out of 2), float
float max_2f(float a, float b) {

    return (a >= b ? a : b);
}


// Order three points, compare y axis
void order_points_y_3(_POINT* a, _POINT* b, _POINT* c) {

    _POINT p[3];
    p[0] = *a;
    p[1] = *b;
    p[2] = *c;
    int i = 0;

    _POINT buf;
    bool changed = false;
    do {

        changed = false;
        for(i=1; i < 3; ++ i) {

            if(p[i].y < p[i-1].y) {

                buf = p[i];
                p[i] = p[i-1];
                p[i-1] = buf;
                changed = true;
                break;
            }
            
        }

    }
    while(changed);

    *a = p[0];
    *b = p[1];
    *c = p[2];
    
}

// Is inside triangle
bool inside_triangle(int px, int py, int x1, int y1, int x2, int y2, int x3, int y3) {

    int as_x = px-x1;
    int as_y = py-y1;
 
    bool s_ab = (x2-x1)*as_y-(y2-y1)*as_x > 0;
 
    if( ((x3-x1)*as_y-(y3-y1)*as_x > 0) == s_ab) return false;
    if( ((x3-x2)*(py-y2)-(y3-y2)*(px-x2) > 0) != s_ab) return false;
 
    return true;
}