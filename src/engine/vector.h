// GOAT
// Vector & point types (header)
// (c) 2018 Jani Nykänen

#ifndef __VECTOR__
#define __VECTOR__

// Vector 2 type
typedef struct {

    float x,y;
}
VEC2;

// Vector 3 type
typedef struct {

    float x,y,z;
}
VEC3;

// Point type
typedef struct {

    int x,y;
}
_POINT;

// 2x2 matrix
typedef struct {

    float m11, m21;
    float m12, m22;
}
MATRIX2;

// 2x2 matrix (integer)
typedef struct {

    int m11, m21;
    int m12, m22;
}
MATRIX2INT;

// Create a 2x2 matrix
#define MATRIX2(b11,b21,b12,b22) (MATRIX2){b11,b21,b12,b22}

// Create a 2x2 matrix (integer)
#define MATRIX2int(b11,b21,b12,b22) (MATRIX2INT){b11,b21,b12,b22}

// Vector 2 constructor
VEC2 vec2(float x, float y);

// Vector 3 constructor
VEC3 vec3(float x, float y, float z);

// Point constructor
_POINT point(int x, int y);

// Addition for vector
VEC2 vec2_add(VEC2 a, VEC2 b);

// Addition for point
_POINT point_add(_POINT a, _POINT b);

// Calculate determinant
float MATRIX2_det(MATRIX2 m);

// Inverse matrix
MATRIX2 MATRIX2_inverse(MATRIX2 m);

// Matrix multiplication
MATRIX2 MATRIX2_mul(MATRIX2 a, MATRIX2 b);

// Multiply matrix with a vector
VEC2 MATRIX2_mul_vec2(MATRIX2 m, VEC2 v);

// Float to int, matrices
MATRIX2INT MATRIX2_to_int(MATRIX2 m);

#endif // __VECTOR__