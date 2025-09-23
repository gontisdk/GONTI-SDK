#ifndef VECTOR3_H
#define VECTOR3_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../../Maths.h"
        #include "../VectorsTypes.inl"

        /* VEC2 */
        KAPI vec3 gontiVec3Create(f32 x, f32 y, f32 z);
        KAPI vec3 gontiVec3FromVec4(vec4 v);
        KAPI vec3 gontiVec3Zero();
        KAPI vec3 gontiVec3One();
        KAPI vec3 gontiVec3Up();
        KAPI vec3 gontiVec3Down();
        KAPI vec3 gontiVec3Left();
        KAPI vec3 gontiVec3Right();
        KAPI vec3 gontiVec3Forward();
        KAPI vec3 gontiVec3Backward();
        KAPI vec3 gontiVec3Add(vec3 v1, vec3 v2);
        KAPI vec3 gontiVec3Subtract(vec3 v1, vec3 v2);
        KAPI vec3 gontiVec3Multiply(vec3 v1, vec3 v2);
        KAPI vec3 gontiVec3MultiplyScalar(vec3 v, f32 scalar);
		KAPI vec3 gontiVec3Divide(vec3 v1, vec3 v2);
        KAPI vec3 gontiVec3Normalized(vec3 v);
        KAPI vec3 gontiVec3Cross(vec3 v1, vec3 v2);

        /* VEC4 */
        KAPI vec4 gontiVec3ToVec4(vec3 v, f32 w);

        /* FLOAT / F32 */
        KAPI f32 gontiVec3LengthSquared(vec3 v);
        KAPI f32 gontiVec3Length(vec3 v);
        KAPI f32 gontiVec3Distance(vec3 v1, vec3 v2);
        KAPI f32 gontiVec3Dot(vec3 v1, vec3 v2);

        /* VOID */
        KAPI void gontiVec3Normalize(vec3* v);

        /* BOOL / B8 */
        KAPI b8 gontiVec3Compare(vec3 v1, vec3 v2, f32 tolerance);

#ifdef __cplusplus
    }
#endif

#endif