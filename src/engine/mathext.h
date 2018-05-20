// GOAT
// Math extensions (header)
// (c) 2018 Jani Nykänen

#ifndef __MATHEXT__
#define __MATHEXT__

#include <stdbool.h>
#include "vector.h"

#ifndef M_PI
#define M_PI 3.14159265359f
#endif 

// Minimum (out of 3)
int min_3(int a, int b, int c);

// Maximum (out of 3)
int max_3(int a, int b, int c);

// Minimum (out of 3), float
float min_3f(float a, float b, float c);

// Maximum (out of 3), float
float max_3f(float a, float b, float c);

// Minimum (out of 2)
int min_2(int a, int b);

// Maximum (out of 2)
int max_2(int a, int b);

// Minimum (out of 2), float
float min_2f(float a, float b);

// Maximum (out of 2), float
float max_2f(float a, float b);

// Order three points, compare y axis
void order_points_y_3(POINT* a, POINT* b, POINT* c);

// Is inside triangle
bool inside_triangle(int px, int py, int x1, int y1, int x2, int y2, int x3, int y3);

#endif // __MATHEXT__
