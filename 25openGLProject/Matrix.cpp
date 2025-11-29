#include "matrix.h"
#include <cmath>

void MatIdentity(float M[16]) {
    for (int i = 0; i < 16; i++) M[i] = 0;
    M[0] = M[5] = M[10] = M[15] = 1.0f;
}

void MatMul(float R[16], const float A[16], const float B[16]) {
    float t[16];
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            t[r * 4 + c] =
            A[r * 4 + 0] * B[0 * 4 + c] +
            A[r * 4 + 1] * B[1 * 4 + c] +
            A[r * 4 + 2] * B[2 * 4 + c] +
            A[r * 4 + 3] * B[3 * 4 + c];

    for (int i = 0; i < 16; i++) R[i] = t[i];
}

void MatTranslate(float M[16], float x, float y, float z) {
    MatIdentity(M);
    M[12] = x;
    M[13] = y;
    M[14] = z;
}

void MatRotateY(float M[16], float angle) {
    float c = cos(angle), s = sin(angle);
    MatIdentity(M);
    M[0] = c;  M[2] = s;
    M[8] = -s; M[10] = c;
}

void MatRotateX(float M[16], float angle) {
    float c = cos(angle), s = sin(angle);
    MatIdentity(M);
    M[5] = c;   M[6] = -s;
    M[9] = s;   M[10] = c;
}
void MatRotateZ(float* m, float rad)
{
    float c = cos(rad);
    float s = sin(rad);

    m[0] = c;   m[4] = -s;  m[8] = 0;  m[12] = 0;
    m[1] = s;   m[5] = c;  m[9] = 0;  m[13] = 0;
    m[2] = 0;   m[6] = 0;  m[10] = 1; m[14] = 0;
    m[3] = 0;   m[7] = 0;  m[11] = 0; m[15] = 1;
}

void MatScale(float M[16], float sx, float sy, float sz)
{
    MatIdentity(M);
    M[0] = sx;
    M[5] = sy;
    M[10] = sz;
}
void MatPerspective(float M[16], float fovDeg, float aspect, float nearZ, float farZ) {
    float fovRad = fovDeg * 3.141592f / 180.0f;
    float f = 1.0f / tan(fovRad / 2.0f);
    MatIdentity(M);

    M[0] = f / aspect;
    M[5] = f;
    M[10] = (farZ + nearZ) / (nearZ - farZ);
    M[11] = -1.0f;
    M[14] = (2 * farZ * nearZ) / (nearZ - farZ);
    M[15] = 0;
}

void MatLookAt(float M[16],
    float ex, float ey, float ez,
    float cx, float cy, float cz,
    float ux, float uy, float uz)
{
    float F[3] = { cx - ex, cy - ey, cz - ez };
    float fLen = sqrt(F[0] * F[0] + F[1] * F[1] + F[2] * F[2]);
    F[0] /= fLen; F[1] /= fLen; F[2] /= fLen;

    float UP[3] = { ux, uy, uz };
    float upLen = sqrt(UP[0] * UP[0] + UP[1] * UP[1] + UP[2] * UP[2]);
    UP[0] /= upLen; UP[1] /= upLen; UP[2] /= upLen;

    float S[3] = {
        F[1] * UP[2] - F[2] * UP[1],
        F[2] * UP[0] - F[0] * UP[2],
        F[0] * UP[1] - F[1] * UP[0]
    };
    float sLen = sqrt(S[0] * S[0] + S[1] * S[1] + S[2] * S[2]);
    S[0] /= sLen; S[1] /= sLen; S[2] /= sLen;

    float U[3] = {
        S[1] * F[2] - S[2] * F[1],
        S[2] * F[0] - S[0] * F[2],
        S[0] * F[1] - S[1] * F[0]
    };

    MatIdentity(M);

    M[0] = S[0]; M[4] = S[1]; M[8] = S[2];
    M[1] = U[0]; M[5] = U[1]; M[9] = U[2];
    M[2] = -F[0]; M[6] = -F[1]; M[10] = -F[2];

    M[12] = -(S[0] * ex + S[1] * ey + S[2] * ez);
    M[13] = -(U[0] * ex + U[1] * ey + U[2] * ez);
    M[14] = (F[0] * ex + F[1] * ey + F[2] * ez);
}
