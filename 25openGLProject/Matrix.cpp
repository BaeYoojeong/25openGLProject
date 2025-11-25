#include "Matrix.h"
#include <cmath>
#include <cstring>

void MatIdentity(float M[16]) {
    memset(M, 0, sizeof(float) * 16);
    M[0] = M[5] = M[10] = M[15] = 1;
}

void MatMul(float R[16], const float A[16], const float B[16]) {
    float t[16];
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            t[r * 4 + c] =
            A[r * 4 + 0] * B[c + 0] +
            A[r * 4 + 1] * B[c + 4] +
            A[r * 4 + 2] * B[c + 8] +
            A[r * 4 + 3] * B[c + 12];
    memcpy(R, t, sizeof(t));
}

void MatTranslate(float M[16], float x, float y, float z) {
    MatIdentity(M);
    M[12] = x;  M[13] = y;  M[14] = z;
}

void MatRotateY(float M[16], float a) {
    float c = cos(a), s = sin(a);
    MatIdentity(M);
    M[0] = c; M[2] = s;
    M[8] = -s; M[10] = c;
}

void MatRotateX(float M[16], float a) {
    float c = cos(a), s = sin(a);
    MatIdentity(M);
    M[5] = c; M[6] = -s;
    M[9] = s; M[10] = c;
}
void MatRotateZ(float M[16], float angle)
{
    float c = cos(angle);
    float s = sin(angle);

    MatIdentity(M);

    M[0] = c;   M[1] = -s;
    M[4] = s;   M[5] = c;
}

void MatPerspective(float M[16], float fov, float asp, float n, float f) {
    float t = tan(fov * 3.141592f / 180 / 2);
    MatIdentity(M);
    M[0] = 1 / (asp * t);
    M[5] = 1 / t;
    M[10] = -(f + n) / (f - n);
    M[11] = -1;
    M[14] = -(2 * f * n) / (f - n);
    M[15] = 0;
}
void MatScale(float M[16], float sx, float sy, float sz)
{
    MatIdentity(M);
    M[0] = sx;
    M[5] = sy;
    M[10] = sz;
}

void MatLookAt(float M[16],
    float ex, float ey, float ez,
    float cx, float cy, float cz,
    float ux, float uy, float uz)
{
    float F[3] = { cx - ex,cy - ey,cz - ez };
    float fLen = sqrt(F[0] * F[0] + F[1] * F[1] + F[2] * F[2]);
    F[0] /= fLen; F[1] /= fLen; F[2] /= fLen;

    float Up[3] = { ux,uy,uz };
    float uLen = sqrt(ux * ux + uy * uy + uz * uz);
    Up[0] /= uLen; Up[1] /= uLen; Up[2] /= uLen;

    float S[3] = {
        F[1] * Up[2] - F[2] * Up[1],
        F[2] * Up[0] - F[0] * Up[2],
        F[0] * Up[1] - F[1] * Up[0]
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
