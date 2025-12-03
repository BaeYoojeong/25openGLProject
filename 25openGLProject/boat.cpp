#include "boat.h"
#include "globals.h"
#include "matrix.h"
#include <cstring>
#include <cmath>

// ===============================
// º¸Æ® ¸ðµ¨ Çà·Ä ¾÷µ¥ÀÌÆ®
// ===============================
void UpdateBoatModel() {

    float rad = boatRotY * M_PI / 180.0f;

    float RfixX1[16], RfixX2[16], Rfix[16];
    MatRotateX(RfixX1, -M_PI / 2.0f);
    MatRotateX(RfixX2, M_PI);
    MatMul(Rfix, RfixX2, RfixX1);

    float S[16], RX[16], RY[16], RZ[16], T[16];
    float tmpA[16], tmpB[16], tmpC[16], tmpD[16], model[16];

    MatRotateY(RY, rad);
    MatRotateZ(RZ, boatRotZ * M_PI / 180.0f);
    MatScale(S, boatScale, boatScale, -boatScale);

    float Bfix[16];
    MatRotateX(Bfix, -M_PI / 2.0f);

    MatMul(tmpA, Rfix, RY);
    MatMul(tmpB, tmpA, RZ);
    MatMul(tmpC, tmpB, Bfix);
    MatMul(tmpD, tmpC, S);

    MatTranslate(T, boatPosX, boatPosY, boatPosZ);
    MatMul(model, T, tmpD);

    memcpy(gModel, model, sizeof(float) * 16);
}

// ===============================
// ¿À¸¥ÂÊ ³ë ¸ðµ¨
// ===============================
void UpdateRightOarModel() {

    float rad = boatRotY * M_PI / 180.0f;

    float BfixOar[16];
    MatRotateX(BfixOar, -M_PI / 2.0f);

    float rFix[16], rBoatRot[16], rRX[16], rRZ[16];
    float rOarRot[16], rScale[16];
    float rTmp1[16], rTmp2[16], rTmp3[16], rT[16], rModel[16];

    MatRotateY(rBoatRot, rad);
    MatRotateZ(rOarRot, rightRotZ * M_PI / 180.0f);
    MatScale(rScale, -boatScale, -boatScale, boatScale);

    MatRotateX(rFix, -M_PI / 2);
    MatRotateX(rRX, boatRotX * M_PI / 180.0f);
    MatRotateZ(rRZ, boatRotZ * M_PI / 180.0f);

    MatMul(rTmp1, rFix, rBoatRot);
    MatMul(rTmp2, rTmp1, rRX);
    MatMul(rTmp3, rTmp2, rRZ);
    MatMul(rTmp1, rTmp3, rOarRot);
    MatMul(rTmp2, rTmp1, BfixOar);
    MatMul(rTmp3, rTmp2, rScale);

    float rOff = 3.0f;
    MatTranslate(rT,
        boatPosX + rOff * cos(rad),
        boatPosY,
        boatPosZ - rOff * sin(rad)
    );

    MatMul(rModel, rT, rTmp3);
    memcpy(gRightModel, rModel, sizeof(float) * 16);
}

// ===============================
// ¿ÞÂÊ ³ë ¸ðµ¨
// ===============================
void UpdateLeftOarModel() {

    float rad = boatRotY * M_PI / 180.0f;

    float BfixOar[16];
    MatRotateX(BfixOar, -M_PI / 2.0f);

    float lFix[16], lBoatRot[16], lRX[16], lRZ[16];
    float lOarRot[16], lScale[16];
    float lTmp1[16], lTmp2[16], lTmp3[16], lT[16], lModel[16];

    MatRotateY(lBoatRot, rad);
    MatRotateZ(lOarRot, leftRotZ * M_PI / 180.0f);
    MatScale(lScale, -boatScale, -boatScale, boatScale);

    MatRotateX(lFix, -M_PI / 2);
    MatRotateX(lRX, boatRotX * M_PI / 180.0f);
    MatRotateZ(lRZ, boatRotZ * M_PI / 180.0f);

    MatMul(lTmp1, lFix, lBoatRot);
    MatMul(lTmp2, lTmp1, lRX);
    MatMul(lTmp3, lTmp2, lRZ);
    MatMul(lTmp1, lTmp3, lOarRot);
    MatMul(lTmp2, lTmp1, BfixOar);
    MatMul(lTmp3, lTmp2, lScale);

    float lOff = 15.0f;
    MatTranslate(lT,
        boatPosX + lOff * cos(rad),
        boatPosY + 5,
        boatPosZ - lOff * sin(rad)
    );

    MatMul(lModel, lT, lTmp3);
    memcpy(gLeftModel, lModel, sizeof(float) * 16);
}
