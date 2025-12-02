#include "Quaternion.h"
#include "../../../Maths.h"
#include "../../Matrices/Matrix4/Matrix4.h"

/*
* QUAT
*/
quat gontiQuatIdentity() {
    return (quat){.elements = {0, 0, 0, 1.0f}};
}
quat gontiQuatNormalize(quat q) {
    f32 n = gontiQuatNormal(q);

    return (quat){.elements = {q.x / n, q.y / n, q.z / n, q.w / n}};
}
quat gontiQuatConjugate(quat q) {
    return (quat){.elements = {-q.x, -q.y, -q.z, -q.w}};
}
quat gontiQuatInverse(quat q) {
    return gontiQuatNormalize(gontiQuatConjugate(q));
}
quat gontiQuatMultiply(quat q1, quat q2) {
    quat ret;

    ret.x = q1.x * q2.w + 
            q1.y * q2.z - 
            q1.z * q2.y + 
            q1.w * q2.x;

    ret.y = -q1.x * q2.z + 
            q1.y * q2.w + 
            q1.z * q2.x + 
            q1.w * q2.y;

    ret.z = q1.x * q2.y - 
            q1.y * q2.x + 
            q1.z * q2.w + 
            q1.w * q2.z;

    ret.w = -q1.x * q2.x - 
            q1.y * q2.y - 
            q1.z * q2.z + 
            q1.w * q2.w;

    return ret;
}
quat gontiQuatFromAxisAngle(vec3 axis, f32 angle, b8 normalize) {
    const f32 halfAngle = 0.5f * angle;
    f32 s = gontiMathSin(halfAngle);
    f32 c = gontiMathCos(halfAngle);

    quat ret = (quat){.elements = {s * axis.x, s * axis.y, s * axis.z, c}};
    if (normalize) return gontiQuatNormalize(ret);

    return ret;
}
quat gontiQuatSlerp(quat q1, quat q2, f32 percentage) {
    quat ret;
    quat v0 = gontiQuatNormalize(q1);
    quat v1 = gontiQuatNormalize(q2);
    f32 dot = gontiQuatDot(v0, v1);

    if (dot < 0.0f) {
        v1.x = -v1.x;
        v1.y = -v1.y;
        v1.z = -v1.z;
        v1.w = -v1.w;
        dot = -dot;
    }

    const f32 DOT_THRESHOLD = 0.9995f;
    if (dot > DOT_THRESHOLD) {
        ret = (quat){.elements = {
            v0.x + ((v1.x - v0.x) * percentage),
            v0.y + ((v1.y - v0.y) * percentage),
            v0.z + ((v1.z - v0.z) * percentage),
            v0.w + ((v1.w - v0.w) * percentage),
        }};
        return gontiQuatNormalize(ret);
    }

    f32 theta0 = gontiMathAcos(dot);
    f32 theta = theta0 * percentage;
    f32 sinTheta = gontiMathSin(theta);
    f32 sinTheta0 = gontiMathSin(theta0);
    f32 s0 = gontiMathCos(theta) - dot * sinTheta / sinTheta0;
    f32 s1 = sinTheta / sinTheta0;

    return (quat){.elements = {
        (v0.x * s0) + (v1.x * s1),
        (v0.y * s0) + (v1.y * s1),
        (v0.z * s0) + (v1.z * s1),
        (v0.w * s0) + (v1.w * s1),
    }};
}

/*
* F32
*/
f32 gontiQuatNormal(quat q) {
    return gontiMathSqrt(
        q.x * q.x +
        q.y * q.y +
        q.z * q.z +
        q.w * q.w
    );
}
f32 gontiQuatDot(quat q1, quat q2) {
    return q1.x * q2.x +
            q1.y * q2.y +
            q1.z * q2.z +
            q1.w * q2.w;
}
f32 gontiQuatDegToRad(f32 degrees) {
    return degrees * K_DEG2RAD_MULTIPLIER;
}
f32 gontiQuatRadToDeg(f32 radians) {
    return radians * K_RAD2DEG_MULTIPLIER;
}

/*
* MAT4
*/
mat4 gontiQuatToMat4(quat q) {
    mat4 ret = gontiMat4Identity();
    quat n = gontiQuatNormalize(q);

    ret.data[0] = 1.0f - 2.0f * n.y * n.y - 2.0f * n.z * n.z;
    ret.data[1] = 2.0f * n.x * n.y - 2.0f * n.z * n.w;
    ret.data[2] = 2.0f * n.x * n.z + 2.0f * n.y * n.w;

    ret.data[4] = 2.0f * n.x * n.y + 2.0f * n.z * n.w;
    ret.data[5] = 1.0f - 2.0f * n.x * n.x - 2.0f * n.z * n.z;
    ret.data[6] = 2.0f * n.y * n.z - 2.0f * n.x * n.w;

    ret.data[8] = 2.0f * n.x * n.z - 2.0f * n.y * n.w;
    ret.data[9] = 2.0f * n.y * n.z + 2.0f * n.x * n.w;
    ret.data[10] = 1.0f - 2.0f * n.x * n.x - 2.0f * n.y * n.y;

    return ret;
}
mat4 gontiQuatToRotationMat4(quat q, vec3 center) {
    mat4 ret;
    f32* o = ret.data;

    o[0] = (q.x * q.x) - (q.y * q.y) - (q.z * q.z) - (q.w * q.w);
    o[1] = 2.0f * ((q.x * q.y) + (q.z * q.w));
    o[2] = 2.0f * ((q.x * q.z) - (q.y * q.w));
    o[3] = center.x - center.x * o[0] - center.y * o[1] - center.z * o[2];

    o[4] = 2.0f * ((q.x * q.y) - (q.z * q.w));
    o[5] = -(q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w);
    o[6] = 2.0f * ((q.y * q.z) + (q.x * q.w));
    o[7] = center.y - center.x * o[4] - center.y * o[5] - center.z * o[6];
    
    o[8] = 2.0f * ((q.x * q.z) + (q.y * q.w));
    o[9] = 2.0f * ((q.y * q.z) - (q.x * q.w));
    o[10] = -(q.x * q.x) - (q.y * q.y) + (q.z * q.z) + (q.w * q.w);
    o[11] = center.z - center.x * o[8] - center.y * o[9] - center.z * o[10];

    o[12] = 0.0f;
    o[13] = 0.0f;
    o[14] = 0.0f;
    o[15] = 1.0f;

    return ret;
}