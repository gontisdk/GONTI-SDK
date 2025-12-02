#ifndef QUATERNION_H
#define QUATERNION_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../QuaternionTypes.inl"
        #include "../../Matrices/MatricesTypes.inl"

        /* QUAT */
        KAPI quat gontiQuatIdentity();
        KAPI quat gontiQuatNormalize(quat q);
        KAPI quat gontiQuatConjugate(quat q);
        KAPI quat gontiQuatInverse(quat q);
        KAPI quat gontiQuatMultiply(quat q1, quat q2);
        KAPI quat gontiQuatFromAxisAngle(vec3 axis, f32 angle, b8 normalize);
        KAPI quat gontiQuatSlerp(quat q1, quat q2, f32 percentage);

        /* F32 */
        KAPI f32 gontiQuatNormal(quat q);
        KAPI f32 gontiQuatDot(quat q1, quat q2);
        KAPI f32 gontiQuatDegToRad(f32 degrees);
        KAPI f32 gontiQuatRadToDeg(f32 radians);

        /* MAT4 */
        KAPI mat4 gontiQuatToMat4(quat q);
        KAPI mat4 gontiQuatToRotationMat4(quat q, vec3 center);

#ifdef __cplusplus
    }
#endif

#endif