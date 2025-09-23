#ifndef VECTOR2_H
#define VECTOR2_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../../Maths.h"
        #include "../VectorsTypes.inl"

        /* VEC2 */
        KAPI vec2 gontiVec2Create(f32 x, f32 y);
        KAPI vec2 gontiVec2Zero();
        KAPI vec2 gontiVec2One();
        KAPI vec2 gontiVec2Up();
        KAPI vec2 gontiVec2Down();
        KAPI vec2 gontiVec2Left();
        KAPI vec2 gontiVec2Right();
        KAPI vec2 gontiVec2Add(vec2 v1, vec2 v2);
        KAPI vec2 gontiVec2Subtract(vec2 v1, vec2 v2);
        KAPI vec2 gontiVec2Multiply(vec2 v1, vec2 v2);
		KAPI vec2 gontiVec2Divide(vec2 v1, vec2 v2);
        KAPI vec2 gontiVec2Normalized(vec2 v);

        /* FLOAT / F32 */
        KAPI f32 gontiVec2LengthSquared(vec2 v);
        KAPI f32 gontiVec2Length(vec2 v);
        KAPI f32 gontiVec2Distance(vec2 v1, vec2 v2);

        /* VOID */
        KAPI void gontiVec2Normalize(vec2* v);

        /* BOOL / B8 */
        KAPI b8 gontiVec2Compare(vec2 v1, vec2 v2, f32 tolerance);

#ifdef __cplusplus
    }
#endif

#endif