#pragma once
#include <GL/glew.h>
#include <vector>
#include "objloader.h"

class Boat {
public:
    GLuint vao = 0, vbo = 0;
    int vertexCount = 0;

    bool load(const char* path);
    void draw();
    void release();
};
