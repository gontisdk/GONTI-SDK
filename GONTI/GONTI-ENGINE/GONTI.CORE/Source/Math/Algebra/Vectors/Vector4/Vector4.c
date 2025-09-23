#include "Vector4.h"

/*
* VEC4
*/
vec4 gontiVec4Create(f32 x, f32 y, f32 z, f32 w) {
    vec4 ret;

    #if defined(KUSE_SIMD)
        ret.data = _mm_setr_ps(x, y, z, w);
    #else
        ret.x = x;
        ret.y = y;
        ret.z = z;
        ret.w = w;
    #endif

    return ret;
}
vec4 gontiVec4FromVec3(vec3 v, f32 w) {
    #if defined(KUSE_SIMD)
        vec4 ret;
        ret.data = _mm_setr_ps(v.x, v.y, v.z, w);
        return ret;
    #else
        return (vec4){.elements = {v.x, v.y, v.z, w}};
    #endif
}
vec4 gontiVec4Zero() {
    return (vec4){.elements = {0.0f, 0.0f, 0.0f, 0.0f}};
}
vec4 gontiVec4One() {
    return (vec4){.elements = {1.0f, 1.0f, 1.0f, 1.0f}};
}
vec4 gontiVec4Add(vec4 v1, vec4 v2) {
    vec4 ret;

    for (u64 i = 0; i < 4; i++) {
        ret.elements[i] = v1.elements[i] + v2.elements[i];
    }

    return ret;
}
vec4 gontiVec4Subtract(vec4 v1, vec4 v2) {
    vec4 ret;

    for (u64 i = 0; i < 4; i++) {
        ret.elements[i] = v1.elements[i] - v2.elements[i];
    }

    return ret;
}
vec4 gontiVec4Multiply(vec4 v1, vec4 v2) {
    vec4 ret;

    for (u64 i = 0; i < 4; i++) {
        ret.elements[i] = v1.elements[i] * v2.elements[i];
    }

    return ret;
}
vec4 gontiVec4Divide(vec4 v1, vec4 v2) {
    vec4 ret;

    for (u64 i = 0; i < 4; i++) {
        ret.elements[i] = v1.elements[i] * v2.elements[i];
    }

    return ret;
}
vec4 gontiVec4Normalized(vec4 v) {
    gontiVec4Normalize(&v);
    return v;
}

/*
* VEC3
*/
vec3 gontiVec4ToVec3(vec4 v) {
    return (vec3){.elements = {v.x, v.y, v.z}};
}

/*
* F32 / FLOAT
*/
f32 gontiVec4LengthSquared(vec4 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}
f32 gontiVec4Length(vec4 v) {
    return gontiMathSqrt(gontiVec4LengthSquared(v));
}
f32 gontiVec4DotF32(f32 a0, f32 a1, f32 a2, f32 a3, f32 b0, f32 b1, f32 b2, f32 b3) {
    f32 ret;
    
    ret =
        a0 * b0 +
        a1 * b1 +
        a2 * b2 +
        a3 * b3;

    return ret;
}

/*
* VOID
*/
void gontiVec4Normalize(vec4* v) {
    const f32 length = gontiVec4Length(*v);
    v->x /= length;
    v->y /= length;
    v->z /= length;
    v->w /= length;
}