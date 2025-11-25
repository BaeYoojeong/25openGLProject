#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <cmath>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include "Matrix.h"
#include "Shader.h"
#include "OBJLoader.h"
#include "Boat.h"

using namespace std;

// ------------------------
// Camera
// ------------------------
float camYaw = 0.0f;
float camPitch = 0.0f;
float camDistance = 120.0f;

int lastMouseX = 0;
int lastMouseY = 0;
bool mouseDown = false;

// boat object
Boat boat;

// ------------------------
// Camera position calc
// ------------------------
void GetCameraPos(float& x, float& y, float& z)
{
    float cx = cos(camYaw) * cos(camPitch);
    float cy = sin(camPitch);
    float cz = sin(camYaw) * cos(camPitch);

    x = cx * camDistance;
    y = cy * camDistance;
    z = cz * camDistance;
}


// ------------------------
// Mouse callbacks
// ------------------------
void mouseButton(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN) {
            mouseDown = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else if (state == GLUT_UP) {
            mouseDown = false;
        }
    }
}

void mouseMotion(int x, int y)
{
    if (!mouseDown) return;

    int dx = x - lastMouseX;
    int dy = y - lastMouseY;

    camYaw += dx * 0.005f;
    camPitch += dy * 0.005f;

    if (camPitch > 1.5f) camPitch = 1.5f;
    if (camPitch < -1.5f) camPitch = -1.5f;

    lastMouseX = x;
    lastMouseY = y;
}


// ------------------------
// Display
// ------------------------
GLuint shader;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader);

    float view[16], proj[16];

    // camera
    float ex, ey, ez;
    GetCameraPos(ex, ey, ez);

    MatLookAt(view,
        ex, ey, ez,    // eye
        0, 0, 0,       // center
        0, 1, 0        // up
    );

    MatPerspective(proj,
        45.0f,
        1280.0f / 720.0f,
        0.1f,
        2000.0f
    );

    glUniformMatrix4fv(glGetUniformLocation(shader, "uView"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader, "uProj"), 1, GL_FALSE, proj);

    // Draw boat
    boat.draw(shader);

    glutSwapBuffers();
}


// ------------------------
// Idle
// ------------------------
void idle()
{
    boat.update(0.016f);
    glutPostRedisplay();
}


// ------------------------
// Main
// ------------------------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("OBJ Boat Viewer");

    glewInit();
    glEnable(GL_DEPTH_TEST);

    // Load boat
    if (!boat.load("boat.obj"))
        cout << "Failed to load OBJ\n";

    boat.scale = 0.5f;
    boat.posZ = -20.0f;

    // Shader
    shader = LoadShader("vertex.glsl", "fragment.glsl");

    // callbacks
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);

    glutMainLoop();
    return 0;
}
