#ifndef MATRIX4_H
#define MATRIX4_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../MatricesTypes.inl"
        #include "../../../Maths.h"

        /* MAT4 */
        KAPI mat4 gontiMat4Identity();
        KAPI mat4 gontiMat4Multiplication(mat4 m1, mat4 m2);
        KAPI mat4 gontiMat4Orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 nearClip, f32 farClip);
        KAPI mat4 gontiMat4Perspective(f32 fovRadians, f32 aspectRatio, f32 nearClip, f32 farClip);
        KAPI mat4 gontiMat4LookAt(vec3 position, vec3 target, vec3 up);
        KAPI mat4 gontiMat4Inverse(mat4 m);
        KAPI mat4 gontiMat4Transposed(mat4 m);
        KAPI mat4 gontiMat4Translation(vec3 position);
        KAPI mat4 gontiMat4Scale(vec3 scale);
        KAPI mat4 gontiMat4EulerX(f32 angle_radians);
        KAPI mat4 gontiMat4EulerY(f32 angle_radians);
        KAPI mat4 gontiMat4EulerZ(f32 angle_radians);
        KAPI mat4 gontiMat4EulerXYZ(f32 xRadians, f32 yRadians, f32 zRadians);

        /* VEC3 */
        KAPI vec3 gontiMat4Forward(mat4 m);
        KAPI vec3 gontiMat4Backward(mat4 m);
        KAPI vec3 gontiMat4Up(mat4 m);
        KAPI vec3 gontiMat4Down(mat4 m);
        KAPI vec3 gontiMat4Left(mat4 m);
        KAPI vec3 gontiMat4Right(mat4 m);

#ifdef __cplusplus
    }
#endif

#endif