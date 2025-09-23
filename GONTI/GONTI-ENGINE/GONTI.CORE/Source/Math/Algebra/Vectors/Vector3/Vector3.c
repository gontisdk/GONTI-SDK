#include "Vector3.h"

/*
* VEC3
*/
vec3 gontiVec3Create(f32 x, f32 y, f32 z) {
    return (vec3){.elements = {x, y, z}};
}
vec3 gontiVec3FromVec4(vec4 v) {
    return (vec3){.elements = {v.x, v.y, v.z}};
}
vec3 gontiVec3Zero() {
    return (vec3){.elements = {0.0f, 0.0f, 0.0f}};
}
vec3 gontiVec3One() {
    return (vec3){.elements = {1.0f, 1.0f, 1.0f}};
}
vec3 gontiVec3Up() {
    return (vec3){.elements = {0.0f, 1.0f, 0.0f}};
}
vec3 gontiVec3Down() {
    return (vec3){.elements = {0.0f, -1.0f, 0.0f}};
}
vec3 gontiVec3Left() {
    return (vec3){.elements = {-1.0f, 0.0f, 0.0f}};
}
vec3 gontiVec3Right() {
    return (vec3){.elements = {1.0f, 0.0f, 0.0f}};
}
vec3 gontiVec3Forward() {
    return (vec3){.elements = {0.0f, 0.0f, -1.0f}};
}
vec3 gontiVec3Backward() {
    return (vec3){.elements = {0.0f, 0.0f, 1.0f}};
}
vec3 gontiVec3Add(vec3 v1, vec3 v2) {
    return (vec3){
        .elements = {
            v1.x + v2.x,
            v1.y + v2.y,
            v1.z + v2.z
        }
    };
}
vec3 gontiVec3Subtract(vec3 v1, vec3 v2) {
    return (vec3){
        .elements = {
            v1.x - v2.x,
            v1.y - v2.y,
            v1.z - v2.z
        }
    };
}
vec3 gontiVec3Multiply(vec3 v1, vec3 v2) {
    return (vec3){
        .elements = {
            v1.x * v2.x,
            v1.y * v2.y,
            v1.z * v2.z
        }
    };
}
vec3 gontiVec3MultiplyScalar(vec3 v, f32 scalar) {
    return (vec3){
        .elements = {
            v.x * scalar,
            v.y * scalar,
            v.z * scalar
        }
    };
}
vec3 gontiVec3Divide(vec3 v1, vec3 v2) {
    return (vec3){
        .elements = {
            v1.x / v2.x,
            v1.y / v2.y,
            v1.z / v2.z
        }
    };
}
vec3 gontiVec3Normalized(vec3 v) {
    gontiVec3Normalize(&v);
    return v;
}
vec3 gontiVec3Cross(vec3 v1, vec3 v2) {
    return (vec3){
        .elements = {
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x
        }
    };
}

/*
* VEC4
*/
vec4 gontiVec3ToVec4(vec3 v, f32 w) {
    return (vec4){.elements = {v.x, v.y, v.z, w}};
}

/*
* F32 / FLOAT
*/
f32 gontiVec3LengthSquared(vec3 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}
f32 gontiVec3Length(vec3 v) {
    return gontiMathSqrt(gontiVec3LengthSquared(v));
}
f32 gontiVec3Distance(vec3 v1, vec3 v2) {
    vec3 d = (vec3) {
        .elements = {
            v1.x - v2.x,
            v1.y - v2.y,
            v1.z - v2.z
        }
    };

    return gontiVec3Length(d);
}
f32 gontiVec3Dot(vec3 v1, vec3 v2) {
    f32 ret = 0;

    ret += v1.x * v2.x;
    ret += v1.y * v2.y;
    ret += v1.z * v2.z;

    return ret;
}

/*
* VOID
*/
void gontiVec3Normalize(vec3* v) {
    const f32 length = gontiVec3Length(*v);
    v->x /= length;
    v->y /= length;
    v->z /= length;
}

/*
* B8 / BOOL
*/
b8 gontiVec3Compare(vec3 v1, vec3 v2, f32 tolerance) {
    if (gontiMathAbs(v1.x - v2.x) > tolerance)
        return false;

    if (gontiMathAbs(v1.y - v2.y) > tolerance)
        return false;
    
    if (gontiMathAbs(v1.z - v2.z) > tolerance)
        return false;

    return true;
}
