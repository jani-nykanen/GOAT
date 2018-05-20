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
POINT;

// 2x2 matrix
typedef struct {

    float m11, m21;
    float m12, m22;
}
MAT2;

// 2x2 matrix (integer)
typedef struct {

    int m11, m21;
    int m12, m22;
}
MAT2INT;

// Create a 2x2 matrix
#define mat2(b11,b21,b12,b22) (MAT2){b11,b21,b12,b22}

// Create a 2x2 matrix (integer)
#define mat2int(b11,b21,b12,b22) (MAT2INT){b11,b21,b12,b22}

// Vector 2 constructor
VEC2 vec2(float x, float y);

// Vector 3 constructor
VEC3 vec3(float x, float y, float z);

// Point constructor
POINT point(int x, int y);

// Addition for vector
VEC2 vec2_add(VEC2 a, VEC2 b);

// Addition for point
POINT point_add(POINT a, POINT b);

// Calculate determinant
float mat2_det(MAT2 m);

// Inverse matrix
MAT2 mat2_inverse(MAT2 m);

// Matrix multiplication
MAT2 mat2_mul(MAT2 a, MAT2 b);

// Multiply matrix with a vector
VEC2 mat2_mul_vec2(MAT2 m, VEC2 v);

// Float to int, matrices
MAT2INT mat2_to_int(MAT2 m);

#endif // __VECTOR__