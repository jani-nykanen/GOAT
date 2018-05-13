// <Insert project name here>
// Vector & point types (header)
// (c) 2018 Jani Nykänen

#include "vector.h"


// Vector constructor
VEC2 vec2(float x, float y) {

    return (VEC2){x,y};
}


// Vector 3 constructor
VEC3 vec3(float x, float y, float z) {

    return (VEC3){x,y,z};
}


// Point constructor
POINT point(int x, int y) {

    return (POINT){x,y};
}


// Addition for vector
VEC2 vec2_add(VEC2 a, VEC2 b) {

    return vec2(a.x+b.x, a.y+b.y);
}


// Addition for point
POINT point_add(POINT a, POINT b) {

    return point(a.x+b.x, a.y+b.y);
}


// Calculate determinant
float mat2_det(MAT2 m) {

    return m.m11 * m.m22 - m.m12 * m.m21;
}


// Inverse matrix
MAT2 mat2_inverse(MAT2 m) {

    float d = 1.0f / mat2_det(m);
    MAT2 i;
    i.m11 = d * (m.m22); i.m21 = d * -m.m21;
    i.m12 = d * -(m.m12); i.m22 = d * m.m11;
    return i;
}


// Matrix multiplication
MAT2 mat2_mul(MAT2 a, MAT2 b) {

    MAT2 r;
    r.m11 =  a.m11 * b.m11 + a.m21 * b.m12;
    r.m21 =  a.m11 * b.m21 + a.m21 * b.m22;
    r.m12 =  a.m12 * b.m11 + a.m22 * b.m12;
    r.m22 =  a.m12 * b.m21 + a.m22 * b.m22;
    return r;
}


// Multiple matrix with a vector
VEC2 mat2_mul_vec2(MAT2 m, VEC2 v) {
    
    VEC2 r;
    r.x =  m.m11 * v.x + m.m21 * v.y;
    r.y =  m.m12 * v.x + m.m22 * v.y;

    return r;
}


// Float to int, matrices
MAT2INT mat2_to_int(MAT2 m) {

    return (MAT2INT) { (int)(m.m11 * 1000.0f), (int)(m.m21 * 1000.0f),
        (int)(m.m12 * 1000.0f), (int)(m.m22 * 1000.0f) };
}