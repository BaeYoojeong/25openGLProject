#pragma once
#include <vector>

struct Vertex {
    float px, py, pz;
    float nx, ny, nz;
};

bool LoadOBJ(const char* path, std::vector<Vertex>& mesh);
