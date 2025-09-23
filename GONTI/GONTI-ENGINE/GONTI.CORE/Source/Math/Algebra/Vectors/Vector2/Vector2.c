#include "Vector2.h"

/*
* VEC2
*/
vec2 gontiVec2Create(f32 x, f32 y) {
    return (vec2){.elements = {x, y}};
}
vec2 gontiVec2Zero() {
    return (vec2){.elements = {0.0f, 0.0f}};
}
vec2 gontiVec2One() {
    return (vec2){.elements = {1.0f, 1.0f}};
}
vec2 gontiVec2Up() {
    return (vec2){.elements = {0.0f, 1.0f}};
}
vec2 gontiVec2Down() {
    return (vec2){.elements = {0.0f, -1.0f}};
}
vec2 gontiVec2Left() {
    return (vec2){.elements = {-1.0f, 0.0f}};
}
vec2 gontiVec2Right() {
    return (vec2){.elements = {1.0f, 0.0f}};
}
vec2 gontiVec2Add(vec2 v1, vec2 v2) {
    return (vec2){
        .elements = {
            v1.x + v2.x,
            v1.y + v2.y
        }
    };
}
vec2 gontiVec2Subtract(vec2 v1, vec2 v2) {
    return (vec2){
        .elements = {
            v1.x - v2.x,
            v1.y - v2.y
        }
    };
}
vec2 gontiVec2Multiply(vec2 v1, vec2 v2) {
    return (vec2){
        .elements = {
            v1.x * v2.x,
            v1.y * v2.y
        }
    };
}
vec2 gontiVec2Divide(vec2 v1, vec2 v2) {
    return (vec2){
        .elements = {
            v1.x / v2.x,
            v1.y / v2.y
        }
    };
}
vec2 gontiVec2Normalized(vec2 v) {
    gontiVec2Normalize(&v);
    return v;
}

/*
* F32 / FLOAT
*/
f32 gontiVec2LengthSquared(vec2 v) {
    return v.x * v.x + v.y * v.y;
}
f32 gontiVec2Length(vec2 v) {
    return gontiMathSqrt(gontiVec2LengthSquared(v));
}
f32 gontiVec2Distance(vec2 v1, vec2 v2) {
    vec2 d = (vec2) {
        .elements = {
            v1.x - v2.x,
            v1.y - v2.y
        }
    };

    return gontiVec2Length(d);
}

/*
* VOID
*/
void gontiVec2Normalize(vec2* v) {
    const f32 length = gontiVec2Length(*v);
    v->x /= length;
    v->y /= length;
}

/*
* B8 / BOOL
*/
b8 gontiVec2Compare(vec2 v1, vec2 v2, f32 tolerance) {
    if (gontiMathAbs(v1.x - v2.x) > tolerance)
        return false;

    if (gontiMathAbs(v1.y - v2.y) > tolerance)
        return false;
    
    return true;
}