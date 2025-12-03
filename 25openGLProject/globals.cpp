#include "globals.h"

// ========= VAO/VBO =========
GLuint boatVAO, boatVBO;
GLuint rightVAO, rightVBO;
GLuint leftVAO, leftVBO;
GLuint groundVAO, groundVBO;
GLuint shader;

// ========= Mesh =========
std::vector<Vertex> boatMesh;
std::vector<Vertex> rightMesh;
std::vector<Vertex> leftMesh;

// ========= Model Matrices =========
float gModel[16];
float gRightModel[16];
float gLeftModel[16];

// ========= Boat State =========
float boatRotY = 0.0f;
float boatRotZ = 0.0f;
float boatRotX = 0.0f;
float boatScale = 0.04f;

float boatPosX = 0.0f;
float boatPosY = 0.0f;
float boatPosZ = 0.0f;

// ========= Oars =========
float rightRotZ = 0.0f;
float rightVelZ = 0.0f;
float leftRotZ = 0.0f;
float leftVelZ = 0.0f;

// ========= Movement =========
float velX = 0.0f;
float velZ = 0.0f;

float accel = 0.40f;
float driftFactor = 0.995f;
float turnAngle = 1.0f;

float speed = 0.0f;
float acceleration = 0.03f;
float deceleration = 0.995f;
float maxSpeed = 2.0f;

bool sPressed = false;
bool kPressed = false;

// ========= Camera =========
float camX = 0.0f;
float camY = 5.0f;
float camZ = 15.0f;
