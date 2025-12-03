#include <GL/glew.h>
#include <GL/freeglut.h>

#include "shader.h"
#include "graphics.h"
#include "globals.h"
#include "boat.h"
#include "app.h"

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("Boat Drift Movement");

    glewInit();
    glEnable(GL_DEPTH_TEST);

    LoadAllOBJ();
    InitAllVAO();
    InitGroundVAO();

    shader = LoadShader("vertex.glsl", "fragment.glsl");

    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    glutKeyboardFunc(Keyboard);
    glutKeyboardUpFunc(KeyUp);

    glutMainLoop();
    return 0;
}
