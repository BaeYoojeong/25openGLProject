#include "Boat.h"
#include <cmath>

Boat::Boat()
{
    posX = posY = posZ = 0.0f;
    rotX = 1.5f;
    rotY = -1.5f; 
    rotZ = 0.0f;
    scale = 0.05f;
}

bool Boat::load(const char* path)
{
    mesh.clear();

    if (!LoadOBJ(path, mesh))
        return false;

    uploadToGPU();
    return true;
}

void Boat::uploadToGPU()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(Vertex), mesh.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Boat::update(float dt)
{
}

void Boat::draw(GLuint shaderID)
{
    glUseProgram(shaderID);

    float T[16], RX[16], RY[16], RZ[16], S[16];
    float temp1[16], temp2[16], model[16];

    MatTranslate(T, posX, posY, posZ);
    MatRotateX(RX, rotX);
    MatRotateY(RY, rotY);
    MatRotateZ(RZ, rotZ);
    MatScale(S, scale, scale, scale);

    MatMul(temp1, RX, RY);
    MatMul(temp2, temp1, RZ);
    MatMul(model, T, temp2);
    MatMul(model, model, S);

    glUniformMatrix4fv(glGetUniformLocation(shaderID, "uModel"), 1, GL_FALSE, model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, mesh.size());
}
