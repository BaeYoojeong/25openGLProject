#define _CRT_SECURE_NO_WARNINGS
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include "matrix.h"
#include "shader.h"
#include "objloader.h"

GLuint VAO, VBO;
GLuint shader;
std::vector<Vertex> mesh;

float camRotY = 0.0f;
float camZ = 150.0f;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader);

    float model[16], view[16], proj[16];
    float rotX[16], rotY[16], temp[16], trans[16];

    MatRotateY(rotY, camRotY);
    MatRotateX(rotX, 90.0f * 3.141592f / 180.0f);
    MatMul(temp, rotX, rotY);
    MatTranslate(trans, 0, 0, -20);
    MatMul(model, trans, temp);

    MatLookAt(view, 0, 0, camZ, 0, 0, 0, 0, 1, 0);
    MatPerspective(proj, 45.0f, 1280.0f / 720.0f, 0.1f, 200.0f);

    glUniformMatrix4fv(glGetUniformLocation(shader, "uModel"), 1, GL_FALSE, model);
    glUniformMatrix4fv(glGetUniformLocation(shader, "uView"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(shader, "uProj"), 1, GL_FALSE, proj);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, mesh.size());

    glutSwapBuffers();
}

void idle() {
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("OBJ Viewer");

    glewInit();
    glEnable(GL_DEPTH_TEST);

    LoadOBJ("boat.obj", mesh);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(Vertex), mesh.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);
    glEnableVertexAttribArray(1);

    shader = LoadShader("vertex.glsl", "fragment.glsl");

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
