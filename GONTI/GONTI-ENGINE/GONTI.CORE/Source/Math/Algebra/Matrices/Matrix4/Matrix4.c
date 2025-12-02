#include "Matrix4.h"
#include "../../../../Memory/Memory.h"
#include "../../Vectors/Vector3/Vector3.h"

/*
* MAT4
*/
mat4 gontiMat4Identity() {
    mat4 ret;
    k_zeroMemory(ret.data, sizeof(f32) * 16);

    ret.data[0] = 1.0f;
    ret.data[5] = 1.0f;
    ret.data[10] = 1.0f;
    ret.data[15] = 1.0f;

    return ret;
}
mat4 gontiMat4Multiplication(mat4 m1, mat4 m2) {
    mat4 ret = gontiMat4Identity();

    const f32* m1Ptr = m1.data;
    const f32* m2Ptr = m2.data;
    f32* retPtr = ret.data;

    for (i32 i = 0; i < 4; i++) {
        for (i32 j = 0; j < 4; j++) {
            *retPtr =
                m1Ptr[0] * m2Ptr[0 + j] +
                m1Ptr[1] * m2Ptr[4 + j] +
                m1Ptr[2] * m2Ptr[8 + j] +
                m1Ptr[3] * m2Ptr[12 + j];
            retPtr++;
        }

        m1Ptr += 4;
    }

    return ret;
}
mat4 gontiMat4Orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 nearClip, f32 farClip) {
    mat4 ret = gontiMat4Identity();

    f32 lr = 1.0f / (left - right);
    f32 bt = 1.0f / (bottom - top);
    f32 nf = 1.0f / (nearClip - farClip);

    ret.data[0] = -2.0f * lr;
    ret.data[5] = -2.0f * bt;
    ret.data[10] = 2.0f * nf;

    ret.data[12] = (left + right) * lr;
    ret.data[13] = (top + bottom) * bt;
    ret.data[14] = (farClip + nearClip) * nf;

    return ret;
}
mat4 gontiMat4Perspective(f32 fovRadians, f32 aspectRatio, f32 nearClip, f32 farClip) {
    f32 halfTanFov = gontiMathTan(fovRadians * 0.5f);

    mat4 ret;
    k_zeroMemory(ret.data, sizeof(f32) * 16);

    ret.data[0] = 1.0f / (aspectRatio * halfTanFov);
    ret.data[5] = 1.0f / halfTanFov;
    ret.data[10] = -((farClip + nearClip) / (farClip - nearClip));
    ret.data[11] = -1.0f;
    ret.data[14] = -((2.0f * farClip * nearClip) / (farClip - nearClip));

    return ret;
}
mat4 gontiMat4LookAt(vec3 position, vec3 target, vec3 up) {
    mat4 ret;

    vec3 zAxis;
    zAxis.x = target.x - position.x;
    zAxis.y = target.y - position.y;
    zAxis.z = target.z - position.z;
    zAxis = gontiVec3Normalized(zAxis);

    vec3 xAxis = gontiVec3Normalized(gontiVec3Cross(zAxis, up));
    vec3 yAxis = gontiVec3Cross(xAxis, zAxis);

    ret.data[0] = xAxis.x;
    ret.data[1] = yAxis.x;
    ret.data[2] = -zAxis.x;
    ret.data[3] = 0;
    ret.data[4] = xAxis.y;
    ret.data[5] = yAxis.y;
    ret.data[6] = -zAxis.y;
    ret.data[7] = 0;
    ret.data[8] = xAxis.z;
    ret.data[9] = yAxis.z;
    ret.data[10] = -zAxis.z;
    ret.data[11] = 0;
    ret.data[12] = gontiVec3Dot(xAxis, position);
    ret.data[13] = gontiVec3Dot(yAxis, position);
    ret.data[14] = gontiVec3Dot(zAxis, position);
    ret.data[15] = 1.0f;

    return ret;
}
mat4 gontiMat4Inverse(mat4 m) {
    const f32* mf = m.data;

    f32 t0 = mf[10] * mf[15];
    f32 t1 = mf[14] * mf[11];
    f32 t2 = mf[6] * mf[15];
    f32 t3 = mf[14] * mf[7];
    f32 t4 = mf[6] * mf[11];
    f32 t5 = mf[10] * mf[7];
    f32 t6 = mf[2] * mf[15];
    f32 t7 = mf[14] * mf[3];
    f32 t8 = mf[2] * mf[11];
    f32 t9 = mf[10] * mf[3];
    f32 t10 = mf[2] * mf[7];
    f32 t11 = mf[6] * mf[3];
    f32 t12 = mf[8] * mf[13];
    f32 t13 = mf[12] * mf[9];
    f32 t14 = mf[4] * mf[13];
    f32 t15 = mf[12] * mf[5];
    f32 t16 = mf[4] * mf[9];
    f32 t17 = mf[8] * mf[5];
    f32 t18 = mf[0] * mf[13];
    f32 t19 = mf[12] * mf[1];
    f32 t20 = mf[0] * mf[9];
    f32 t21 = mf[8] * mf[1];
    f32 t22 = mf[0] * mf[5];
    f32 t23 = mf[4] * mf[1];

    mat4 ret;
    f32* o = ret.data;

    o[0] = (t0 * mf[5] + t3 * mf[9] + t4 * mf[13]) -
           (t1 * mf[5] + t2 * mf[9] + t5 * mf[13]);
    o[1] = (t1 * mf[1] + t6 * mf[9] + t9 * mf[13]) -
           (t0 * mf[1] + t7 * mf[9] + t8 * mf[13]);
    o[2] = (t2 * mf[1] + t7 * mf[5] + t10 * mf[13]) -
           (t3 * mf[1] + t6 * mf[5] + t11 * mf[13]);
    o[3] = (t5 * mf[1] + t8 * mf[5] + t11 * mf[9]) -
           (t4 * mf[1] + t9 * mf[5] + t10 * mf[9]);

    f32 d = 1.0f / (mf[0] * o[0] + mf[4] * o[1] + mf[8] * o[2] + mf[12] * o[3]);

    if (gontiMathAbs(d) < 1e-6f) {
        return gontiMat4Identity();
    }

    o[0] = d * o[0];
    o[1] = d * o[1];
    o[2] = d * o[2];
    o[3] = d * o[3];
    o[4] = d * ((t1 * mf[4] + t2 * mf[8] + t5 * mf[12]) -
                (t0 * mf[4] + t3 * mf[8] + t4 * mf[12]));
    o[5] = d * ((t0 * mf[0] + t7 * mf[8] + t8 * mf[12]) -
                (t1 * mf[0] + t6 * mf[8] + t9 * mf[12]));
    o[6] = d * ((t3 * mf[0] + t6 * mf[4] + t11 * mf[12]) -
                (t2 * mf[0] + t7 * mf[4] + t10 * mf[12]));
    o[7] = d * ((t4 * mf[0] + t9 * mf[4] + t10 * mf[8]) -
                (t5 * mf[0] + t8 * mf[4] + t11 * mf[8]));
    o[8] = d * ((t12 * mf[7] + t15 * mf[11] + t16 * mf[15]) -
                (t13 * mf[7] + t14 * mf[11] + t17 * mf[15]));
    o[9] = d * ((t13 * mf[3] + t18 * mf[11] + t21 * mf[15]) -
                (t12 * mf[3] + t19 * mf[11] + t20 * mf[15]));
    o[10] = d * ((t14 * mf[3] + t19 * mf[7] + t22 * mf[15]) -
                 (t15 * mf[3] + t18 * mf[7] + t23 * mf[15]));
    o[11] = d * ((t17 * mf[3] + t20 * mf[7] + t23 * mf[11]) -
                 (t16 * mf[3] + t21 * mf[7] + t22 * mf[11]));
    o[12] = d * ((t14 * mf[10] + t17 * mf[14] + t13 * mf[6]) -
                 (t16 * mf[14] + t12 * mf[6] + t15 * mf[10]));
    o[13] = d * ((t20 * mf[14] + t12 * mf[2] + t19 * mf[10]) -
                 (t18 * mf[10] + t21 * mf[14] + t13 * mf[2]));
    o[14] = d * ((t18 * mf[6] + t23 * mf[14] + t15 * mf[2]) -
                 (t22 * mf[14] + t14 * mf[2] + t19 * mf[6]));
    o[15] = d * ((t22 * mf[10] + t16 * mf[2] + t21 * mf[6]) -
                 (t20 * mf[6] + t23 * mf[10] + t17 * mf[2]));

    return ret;
}
mat4 gontiMat4Transposed(mat4 m) {
    mat4 ret = gontiMat4Identity();

    ret.data[0] = m.data[0];
    ret.data[1] = m.data[4];
    ret.data[2] = m.data[8];
    ret.data[3] = m.data[12];
    ret.data[4] = m.data[1];
    ret.data[5] = m.data[5];
    ret.data[6] = m.data[9];
    ret.data[7] = m.data[13];
    ret.data[8] = m.data[2];
    ret.data[9] = m.data[6];
    ret.data[10] = m.data[10];
    ret.data[11] = m.data[14];
    ret.data[12] = m.data[3];
    ret.data[13] = m.data[7];
    ret.data[14] = m.data[11];
    ret.data[15] = m.data[15];

    return ret;
}
mat4 gontiMat4Translation(vec3 position) {
    mat4 ret = gontiMat4Identity();

    ret.data[12] = position.x;
    ret.data[13] = position.y;
    ret.data[14] = position.z;

    return ret;
}
mat4 gontiMat4Scale(vec3 scale) {
    mat4 ret = gontiMat4Identity();

    ret.data[0] = scale.x;
    ret.data[5] = scale.y;
    ret.data[10] = scale.z;

    return ret;
}
mat4 gontiMat4EulerX(f32 angle_radians) {
    mat4 ret = gontiMat4Identity();
    f32 c = gontiMathCos(angle_radians);
    f32 s = gontiMathSin(angle_radians);

    ret.data[5] = c;
    ret.data[6] = s;
    ret.data[9] = -s;
    ret.data[10] = c;

    return ret;
}
mat4 gontiMat4EulerY(f32 angle_radians) {
    mat4 ret = gontiMat4Identity();
    f32 c = gontiMathCos(angle_radians);
    f32 s = gontiMathSin(angle_radians);

    ret.data[0] = c;
    ret.data[2] = -s;
    ret.data[8] = s;
    ret.data[10] = c;

    return ret;
}
mat4 gontiMat4EulerZ(f32 angle_radians) {
    mat4 ret = gontiMat4Identity();
    f32 c = gontiMathCos(angle_radians);
    f32 s = gontiMathSin(angle_radians);

    ret.data[0] = c;
    ret.data[1] = s;
    ret.data[4] = -s;
    ret.data[5] = c;

    return ret;
}
mat4 gontiMat4EulerXYZ(f32 xRadians, f32 yRadians, f32 zRadians) {
    mat4 rx = gontiMat4EulerX(xRadians);
    mat4 ry = gontiMat4EulerY(yRadians);
    mat4 rz = gontiMat4EulerZ(zRadians);

    mat4 ret = gontiMat4Multiplication(rx, ry);
    ret = gontiMat4Multiplication(ret, rz);

    return ret;
}

/*
* VEC3
*/
vec3 gontiMat4Forward(mat4 m) {
    vec3 ret;

    ret.x = -m.data[2];
    ret.y = -m.data[6];
    ret.z = -m.data[10];

    gontiVec3Normalize(&ret);
    return ret;
}
vec3 gontiMat4Backward(mat4 m) {
    vec3 ret;

    ret.x = m.data[2];
    ret.y = m.data[6];
    ret.z = m.data[10];

    gontiVec3Normalize(&ret);
    return ret;
}
vec3 gontiMat4Up(mat4 m) {
    vec3 ret;

    ret.x = m.data[1];
    ret.y = m.data[5];
    ret.z = m.data[9];

    gontiVec3Normalize(&ret);
    return ret;
}
vec3 gontiMat4Down(mat4 m) {
    vec3 ret;

    ret.x = -m.data[1];
    ret.y = -m.data[5];
    ret.z = -m.data[9];

    gontiVec3Normalize(&ret);
    return ret;
}
vec3 gontiMat4Left(mat4 m) {
    vec3 ret;

    ret.x = -m.data[0];
    ret.y = -m.data[4];
    ret.z = -m.data[8];

    gontiVec3Normalize(&ret);
    return ret;
}
vec3 gontiMat4Right(mat4 m) {
    vec3 ret;

    ret.x = m.data[0];
    ret.y = m.data[4];
    ret.z = m.data[8];

    gontiVec3Normalize(&ret);
    return ret;
}