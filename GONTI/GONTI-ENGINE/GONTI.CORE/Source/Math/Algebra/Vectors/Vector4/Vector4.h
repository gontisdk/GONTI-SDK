#ifndef VECTOR4_H
#define VECTOR4_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../../Maths.h"
        #include "../VectorsTypes.inl"

        /* VEC4 */
        KAPI vec4 gontiVec4Create(f32 x, f32 y, f32 z, f32 w);
        KAPI vec4 gontiVec4FromVec3(vec3 v, f32 w);
        KAPI vec4 gontiVec4Zero();
        KAPI vec4 gontiVec4One();
        KAPI vec4 gontiVec4Add(vec4 v1, vec4 v2);
        KAPI vec4 gontiVec4Subtract(vec4 v1, vec4 v2);
        KAPI vec4 gontiVec4Multiply(vec4 v1, vec4 v2);
		KAPI vec4 gontiVec4Divide(vec4 v1, vec4 v2);
        KAPI vec4 gontiVec4Normalized(vec4 v);

        /* VEC3 */
        KAPI vec3 gontiVec4ToVec3(vec4 v);

        /* F32 / FLOAT */
        KAPI f32 gontiVec4LengthSquared(vec4 v);
        KAPI f32 gontiVec4Length(vec4 v);
        KAPI f32 gontiVec4DotF32(f32 a0, f32 a1, f32 a2, f32 a3, f32 b0, f32 b1, f32 b2, f32 b3);

        /* VOID */
        KAPI void gontiVec4Normalize(vec4* v);

#ifdef __cplusplus
    }
#endif

#endif