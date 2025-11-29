#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <cmath>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include "matrix.h"
#include "shader.h"
#include "objloader.h"

// =========================
// 모델 데이터
// =========================
GLuint boatVAO, boatVBO;
GLuint rightVAO, rightVBO;
GLuint shader;

std::vector<Vertex> boatMesh;
std::vector<Vertex> rightMesh;

float gModel[16];
float gRightModel[16];   // ★ right.obj 전용 모델 행렬

GLuint leftVAO, leftVBO;
std::vector<Vertex> leftMesh;

float gLeftModel[16];
// =========================
// 보트 상태
// =========================
float boatRotY = 0.0f;
float boatRotX = 0.0f;
float boatRotZ = 0.0f;
float boatScale = 0.04f;

float boatPosX = 0.0f;
float boatPosY = 0.0f;
float boatPosZ = 0.0f;

float rightRotY = 0.0f;   // 회전 각도
float rightVelY = 0.0f;   // 회전 속도

float leftRotY = 0.0f;
float leftVelY = 0.0f;
// =========================
// 카메라 상태
// =========================
bool cameraFollow = false;

float baseCamDist = 12.0f;
float baseCamHeight = 4.0f;

float camX, camY, camZ;
float targetX, targetY, targetZ;

float savedCamX, savedCamY, savedCamZ;
float savedTargetX, savedTargetY, savedTargetZ;

// =========================
// display
// =========================
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader);

    // --- VIEW 행렬 ---
    float view[16];
    MatLookAt(
        view,
        camX, camY, camZ,
        targetX, targetY, targetZ,
        0, 0, 1
    );

    // --- PROJECTION 행렬 ---
    float proj[16];
    MatPerspective(
        proj,
        45.0f,
        1280.0f / 720.0f,
        0.1f,
        500.0f
    );

    glUniformMatrix4fv(glGetUniformLocation(shader, "uView"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader, "uProj"), 1, GL_FALSE, proj);

    // ======================================================
    // 1) boat.obj 렌더링  (원래 네 코드 그대로)
    // ======================================================
    glUniformMatrix4fv(glGetUniformLocation(shader, "uModel"), 1, GL_FALSE, gModel);
    glUniform3f(glGetUniformLocation(shader, "uColor"), 0.8f, 0.8f, 0.8f);

    glBindVertexArray(boatVAO);
    glDrawArrays(GL_TRIANGLES, 0, boatMesh.size());

    // ======================================================
    // 2) right.obj 렌더링 (★ 여기에만 노 젓기용 회전값 적용됨)
    // ======================================================
    glUniformMatrix4fv(glGetUniformLocation(shader, "uModel"), 1, GL_FALSE, gRightModel);
    glUniform3f(glGetUniformLocation(shader, "uColor"), 1.0f, 0.1f, 0.1f);

    glBindVertexArray(rightVAO);
    glDrawArrays(GL_TRIANGLES, 0, rightMesh.size());

    // ======================================================

    // left.obj 그리기
    glUniformMatrix4fv(glGetUniformLocation(shader, "uModel"), 1, GL_FALSE, gLeftModel);
    glUniform3f(glGetUniformLocation(shader, "uColor"), 0.0f, 0.0f, 1.f);
    glBindVertexArray(leftVAO);
    glDrawArrays(GL_TRIANGLES, 0, leftMesh.size());

    glutSwapBuffers();
}


// =========================
// idle
// =========================
void idle() {
    float speed = 0.01f;

    float rad = boatRotY * M_PI / 180.0f;

    float forwardX = sin(rad);
    float forwardY = cos(rad);

    boatPosX += forwardX * speed;
    boatPosY += forwardY * speed;

    float S[16], RX[16], RY[16], RZ[16], T[16];
    float tmpA[16], tmpB[16], tmpC[16], model[16];

    MatRotateY(RY, boatRotY * M_PI / 180.0f);
    MatRotateX(RX, boatRotX * M_PI / 180.0f);
    MatRotateZ(RZ, boatRotZ * M_PI / 180.0f);

    MatScale(S, boatScale, boatScale, boatScale);

    MatMul(tmpA, RY, RX);
    MatMul(tmpB, tmpA, RZ);
    MatMul(tmpC, tmpB, S);
    MatTranslate(T, boatPosX, boatPosY, boatPosZ);
    MatMul(model, T, tmpC);

    memcpy(gModel, model, sizeof(float) * 16);

    // =====================================
// right.obj = 보트 회전 + 노 젓기(Y축 회전)
// =====================================

    // =====================================
// right.obj = 보트 회전 + 노 젓기(Y축 회전)
// =====================================

    float rS[16], rRYb[16], rRXb[16], rRZb[16];   // 보트 회전
    float rRYoar[16];                             // 노젓기 회전(Y축)
    float rA[16], rB[16], rC[16], rD[16], rE[16];
    float rT[16], rModel[16];

    // 보트 회전
    MatRotateY(rRYb, boatRotY * M_PI / 180.0f);
    MatRotateX(rRXb, boatRotX * M_PI / 180.0f);
    MatRotateZ(rRZb, boatRotZ * M_PI / 180.0f);

    // 노젓기(Y축)
    MatRotateY(rRYoar, rightRotY * M_PI / 180.0f);

    // 스케일
    MatScale(rS, boatScale, boatScale, boatScale);

    // 회전 조합
    MatMul(rA, rRYb, rRXb);    // 보트 Y→X
    MatMul(rB, rA, rRZb);      // 보트 Z
    MatMul(rC, rB, rRYoar);    // ★ 노젓기 Y축 회전 추가
    MatMul(rD, rC, rS);        // 스케일 적용

    // 위치 이동
    MatTranslate(rT, boatPosX, boatPosY, boatPosZ);
    MatMul(rModel, rT, rD);

    memcpy(gRightModel, rModel, sizeof(float) * 16);

    // =====================================
// left.obj = 보트 회전 + 노 젓기(Y축 회전)
// =====================================

    float lS[16], lRYb[16], lRXb[16], lRZb[16];
    float lRYoar[16];
    float lA[16], lB[16], lC[16], lD[16];
    float lT[16], lModel[16];

    MatRotateY(lRYb, boatRotY * M_PI / 180.0f);
    MatRotateX(lRXb, boatRotX * M_PI / 180.0f);
    MatRotateZ(lRZb, boatRotZ * M_PI / 180.0f);

    MatRotateY(lRYoar, leftRotY * M_PI / 180.0f);

    MatScale(lS, boatScale, boatScale, boatScale);

    MatMul(lA, lRYb, lRXb);
    MatMul(lB, lA, lRZb);
    MatMul(lC, lB, lRYoar);
    MatMul(lD, lC, lS);

    MatTranslate(lT, boatPosX+20, boatPosY, boatPosZ+5);
    MatMul(lModel, lT, lD);

    memcpy(gLeftModel, lModel, sizeof(float) * 16);


    // ===========================
    // 노 젓기 애니메이션 (Y축 회전)
    // ===========================
    rightRotY += rightVelY;      // 회전 적용
    rightVelY *= 0.85f;          // 감속
    rightVelY += (0.0f - rightRotY) * 0.02f;   // 스프링 복귀

    // 회전 제한
    if (rightRotY > 60) rightRotY = 60;
    if (rightRotY < -60) rightRotY = -60;


    // ===========================
// 왼쪽 노젓기 애니메이션
// ===========================
    leftRotY += leftVelY;
    leftVelY *= 0.85f;
    leftVelY += (0.0f - leftRotY) * 0.02f;

    if (leftRotY > 60) leftRotY = 60;
    if (leftRotY < -60) leftRotY = -60;


    if (cameraFollow) {
        float rad = boatRotY * M_PI / 180.0f;

        // forward 벡터
        float fx = sin(rad);
        float fy = cos(rad);

        // back 벡터 (카메라는 뒤쪽에 있어야 함)
        float bx = fx;
        float by = fy;

        // ★ 드리프트 0% 보장: 위치 기반 재계산
        camX = boatPosX + bx * baseCamDist;
        camY = boatPosY + by * baseCamDist;
        camZ = boatPosZ + baseCamHeight;

        targetX = boatPosX + fx * 3.0f;
        targetY = boatPosY + fy * 3.0f;
        targetZ = boatPosZ;
    }


    else {
        camX = savedCamX;
        camY = savedCamY;
        camZ = savedCamZ;

        targetX = savedTargetX;
        targetY = savedTargetY;
        targetZ = savedTargetZ;
    }

    glutPostRedisplay();
}

// =========================
// 회전
// =========================
void specialKeyboard(int key, int, int) {
    if (key == GLUT_KEY_LEFT) {
        boatRotZ += 2.0f;
        leftVelY -= 4.0f;   // 반대방향
    }

    if (key == GLUT_KEY_RIGHT) {
        boatRotZ -= 2.0f;      // 기존 보트 기울기
        rightVelY += 4.0f;    // → 누르면 Y축으로 오른쪽으로 젓기
    }

    glutPostRedisplay();
}

// =========================
// 키 입력
// =========================
void keyboard(unsigned char key, int, int) {
    switch (key) {
    case 'c':
        cameraFollow = !cameraFollow;
        if (cameraFollow) {
            savedCamX = camX;
            savedCamY = camY;
            savedCamZ = camZ;
            savedTargetX = targetX;
            savedTargetY = targetY;
            savedTargetZ = targetZ;
        }
        break;

    case 'j': boatRotX -= 5; break;
    case 'k': boatRotX += 5; break;
    case 'q': exit(0);
    }
}

// =========================
// main
// =========================
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("Boat Back View Camera + Red Right.obj");

    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);

    // Load OBJ models separately
    LoadOBJ("boat.obj", boatMesh);
    LoadOBJ("right.obj", rightMesh);
    LoadOBJ("left.obj", leftMesh);



    // ---- boat VAO ----
    glGenVertexArrays(1, &boatVAO);
    glBindVertexArray(boatVAO);

    glGenBuffers(1, &boatVBO);
    glBindBuffer(GL_ARRAY_BUFFER, boatVBO);
    glBufferData(GL_ARRAY_BUFFER, boatMesh.size() * sizeof(Vertex), boatMesh.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);
    glEnableVertexAttribArray(1);

    // ---- right VAO ----
    glGenVertexArrays(1, &rightVAO);
    glBindVertexArray(rightVAO);

    glGenBuffers(1, &rightVBO);
    glBindBuffer(GL_ARRAY_BUFFER, rightVBO);
    glBufferData(GL_ARRAY_BUFFER, rightMesh.size() * sizeof(Vertex), rightMesh.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);
    glEnableVertexAttribArray(1);

    // ---- left VAO ----
    glGenVertexArrays(1, &leftVAO);
    glBindVertexArray(leftVAO);

    glGenBuffers(1, &leftVBO);
    glBindBuffer(GL_ARRAY_BUFFER, leftVBO);
    glBufferData(GL_ARRAY_BUFFER, leftMesh.size() * sizeof(Vertex), leftMesh.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);
    glEnableVertexAttribArray(1);

    // Shader
    shader = LoadShader("vertex.glsl", "fragment.glsl");

    // 초기 카메라
    float rad = boatRotY * M_PI / 180.0f;
    float forwardX = sin(rad);
    float forwardY = cos(rad);

    // 카메라 위치 = 보트 위치에서 정확히 뒤쪽으로 오프셋
    camX = boatPosX + forwardX * baseCamDist;
    camY = boatPosY + forwardY * baseCamDist;
    camZ = boatPosZ + baseCamHeight;

    // 바라보는 방향은 보트 앞쪽
    targetX = boatPosX + forwardX * 3.0f;
    targetY = boatPosY + forwardY * 3.0f;
    targetZ = boatPosZ;


    savedCamX = camX;
    savedCamY = camY;
    savedCamZ = camZ;
    savedTargetX = targetX;
    savedTargetY = targetY;
    savedTargetZ = targetZ;

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);

    glutMainLoop();
    return 0;
}
