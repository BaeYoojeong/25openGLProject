#pragma once
#define M_PI 3.14159265358979323846
#include <vector>
#include <GL/glew.h>
#include "objloader.h"

// VAO/VBO
extern GLuint boatVAO, boatVBO;
extern GLuint rightVAO, rightVBO;
extern GLuint leftVAO, leftVBO;
extern GLuint groundVAO, groundVBO;
extern GLuint shader;

// Mesh data
extern std::vector<Vertex> boatMesh;
extern std::vector<Vertex> rightMesh;
extern std::vector<Vertex> leftMesh;

// Model matrices
extern float gModel[16];
extern float gRightModel[16];
extern float gLeftModel[16];

// Boat state
extern float boatRotY;
extern float boatRotZ;
extern float boatRotX;
extern float boatScale;

extern float boatPosX;
extern float boatPosY;
extern float boatPosZ;

// Oars
extern float rightRotZ;
extern float rightVelZ;
extern float leftRotZ;
extern float leftVelZ;

// Movement physics
extern float velX;
extern float velZ;

extern float accel;
extern float driftFactor;
extern float turnAngle;
extern float speed;
extern float acceleration;
extern float deceleration;
extern float maxSpeed;

extern bool sPressed;
extern bool kPressed;

// Camera
extern float camX, camY, camZ;

