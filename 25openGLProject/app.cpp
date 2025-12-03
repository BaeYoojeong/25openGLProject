#include <GL/glew.h>
#include <GL/freeglut.h>
#include "matrix.h"
#include "globals.h"
#include "boat.h"

void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader);

    float view[16];
    MatLookAt(view,
        camX, camY, camZ,
        camX, camY, camZ - 1,
        0, 1, 0
    );

    float proj[16];
    MatPerspective(proj, 45.0f, 1280.0f / 720.0f, 0.1f, 500.0f);

    glUniformMatrix4fv(glGetUniformLocation(shader, "uView"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader, "uProj"), 1, GL_FALSE, proj);

    // -------------------------
    // Ground
    // -------------------------
    float groundModel[16];
    MatIdentity(groundModel);

    glUniformMatrix4fv(glGetUniformLocation(shader, "uModel"), 1, GL_FALSE, groundModel);
    glUniform3f(glGetUniformLocation(shader, "uColor"), 0.1f, 0.3f, 0.8f);

    glBindVertexArray(groundVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // -------------------------
    // Boat
    // -------------------------
    glUniformMatrix4fv(glGetUniformLocation(shader, "uModel"), 1, GL_FALSE, gModel);
    glUniform3f(glGetUniformLocation(shader, "uColor"), 0.8f, 0.8f, 0.8f);

    glBindVertexArray(boatVAO);
    glDrawArrays(GL_TRIANGLES, 0, boatMesh.size());

    // -------------------------
    // Right Oar
    // -------------------------
    glUniformMatrix4fv(glGetUniformLocation(shader, "uModel"), 1, GL_FALSE, gRightModel);
    glUniform3f(glGetUniformLocation(shader, "uColor"), 1.0f, 0.1f, 0.1f);

    glBindVertexArray(rightVAO);
    glDrawArrays(GL_TRIANGLES, 0, rightMesh.size());

    // -------------------------
    // Left Oar
    // -------------------------
    glUniformMatrix4fv(glGetUniformLocation(shader, "uModel"), 1, GL_FALSE, gLeftModel);
    glUniform3f(glGetUniformLocation(shader, "uColor"), 0.1f, 0.1f, 1.0f);

    glBindVertexArray(leftVAO);
    glDrawArrays(GL_TRIANGLES, 0, leftMesh.size());

    glutSwapBuffers();
}


void Idle() {

    float rad = boatRotY * M_PI / 180.0f;
    float dirX = -sin(rad);
    float dirZ = -cos(rad);

    // velocity blending
    velX = velX * 0.9f + dirX * speed * 0.1f;
    velZ = velZ * 0.9f + dirZ * speed * 0.1f;

    boatPosX -= velX;
    boatPosZ -= velZ;

    speed *= deceleration;
    if (speed < 0.0001f) speed = 0;

    // oars
    rightRotZ += rightVelZ;
    rightVelZ *= 0.85f;
    rightVelZ += (0 - rightRotZ) * 0.02f;

    leftRotZ += leftVelZ;
    leftVelZ *= 0.85f;
    leftVelZ += (0 - leftRotZ) * 0.02f;

    // update model matrices
    UpdateBoatModel();
    UpdateRightOarModel();
    UpdateLeftOarModel();

    glutPostRedisplay();
}


void Keyboard(unsigned char key, int x, int y) {

    float turnBoost = 1.0f + speed * 1.2f;
    float turn = turnAngle * turnBoost;

    if (key == 's') {
        if (!sPressed) {
            boatRotZ += turn;
            leftVelZ -= 1;
            speed += acceleration;
            if (speed > maxSpeed)
                speed = maxSpeed;
        }
        sPressed = true;
    }

    if (key == 'k') {
        if (!kPressed) {
            boatRotZ -= turn;
            rightVelZ += 1;
            speed += acceleration;
            if (speed > maxSpeed)
                speed = maxSpeed;
        }
        kPressed = true;
    }
}

void KeyUp(unsigned char key, int x, int y) {
    if (key == 's') sPressed = false;
    if (key == 'k') kPressed = false;
}
