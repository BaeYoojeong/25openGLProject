#pragma once
void MatRotateZ(float M[16], float angle);
void MatScale(float M[16], float sx, float sy, float sz);
void MatIdentity(float M[16]);
void MatMul(float R[16], const float A[16], const float B[16]);
void MatTranslate(float M[16], float x, float y, float z);
void MatRotateX(float M[16], float angle);
void MatRotateY(float M[16], float angle);
void MatPerspective(float M[16], float fovDeg, float aspect, float nearZ, float farZ);
void MatLookAt(float M[16],
    float ex, float ey, float ez,
    float cx, float cy, float cz,
    float ux, float uy, float uz);
