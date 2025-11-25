#pragma once
#include <vector>
#include <GL/glew.h>
#include "OBJLoader.h"
#include "Matrix.h"

class Boat {
public:
    Boat();
    bool load(const char* path);
    void update(float dt);
    void draw(GLuint shaderID);

    float posX, posY, posZ;
    float rotX, rotY, rotZ;
    float scale;

private:
    std::vector<Vertex> mesh;
    GLuint VAO = 0, VBO = 0;

    void uploadToGPU();
};
