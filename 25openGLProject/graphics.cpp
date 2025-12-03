#include "graphics.h"
#include "globals.h"
#include "objloader.h"
#include <GL/glew.h>

void LoadAllOBJ() {

    // ===== BOAT =====
    LoadOBJ("boat.obj", boatMesh);
    for (auto& v : boatMesh) {
        float y = v.py, z = v.pz;
        v.py = z; v.pz = -y;
        float ny = v.ny, nz = v.nz;
        v.ny = nz; v.nz = -ny;
    }

    // ===== RIGHT OAR =====
    LoadOBJ("right.obj", rightMesh);
    for (auto& v : rightMesh) {
        float y = v.py, z = v.pz;
        v.py = z; v.pz = -y;
        float ny = v.ny, nz = v.nz;
        v.ny = nz; v.nz = -ny;
    }

    // ===== LEFT OAR =====
    LoadOBJ("left.obj", leftMesh);
    for (auto& v : leftMesh) {
        float y = v.py, z = v.pz;
        v.py = z; v.pz = -y;
        float ny = v.ny, nz = v.nz;
        v.ny = nz; v.nz = -ny;
    }
}


void InitAllVAO() {

    // ===== BOAT VAO =====
    glGenVertexArrays(1, &boatVAO);
    glBindVertexArray(boatVAO);
    glGenBuffers(1, &boatVBO);
    glBindBuffer(GL_ARRAY_BUFFER, boatVBO);
    glBufferData(GL_ARRAY_BUFFER, boatMesh.size() * sizeof(Vertex),
        boatMesh.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);
    glEnableVertexAttribArray(1);

    // ===== RIGHT OAR VAO =====
    glGenVertexArrays(1, &rightVAO);
    glBindVertexArray(rightVAO);
    glGenBuffers(1, &rightVBO);
    glBindBuffer(GL_ARRAY_BUFFER, rightVBO);
    glBufferData(GL_ARRAY_BUFFER, rightMesh.size() * sizeof(Vertex),
        rightMesh.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);
    glEnableVertexAttribArray(1);

    // ===== LEFT OAR VAO =====
    glGenVertexArrays(1, &leftVAO);
    glBindVertexArray(leftVAO);
    glGenBuffers(1, &leftVBO);
    glBindBuffer(GL_ARRAY_BUFFER, leftVBO);
    glBufferData(GL_ARRAY_BUFFER, leftMesh.size() * sizeof(Vertex),
        leftMesh.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);
    glEnableVertexAttribArray(1);
}

void InitGroundVAO() {
    struct GVertex { float x, y, z, nx, ny, nz; };

    std::vector<GVertex> ground = {
        {-10,0,0,0,1,0}, {10,0,0,0,1,0}, {10,0,-200,0,1,0},
        {-10,0,0,0,1,0}, {10,0,-200,0,1,0}, {-10,0,-200,0,1,0}
    };

    glGenVertexArrays(1, &groundVAO);
    glBindVertexArray(groundVAO);

    glGenBuffers(1, &groundVBO);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, ground.size() * sizeof(GVertex),
        ground.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GVertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GVertex), (void*)12);
    glEnableVertexAttribArray(1);
}
