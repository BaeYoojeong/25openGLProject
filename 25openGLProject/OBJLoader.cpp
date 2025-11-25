#define _CRT_SECURE_NO_WARNINGS

#include "objloader.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool LoadOBJ(const char* path, std::vector<Vertex>& mesh)
{
    std::vector<float> posx, posy, posz;
    std::vector<float> nx, ny, nz;

    std::ifstream file(path);
    if (!file.is_open()) return false;

    std::string line;
    while (getline(file, line)) {
        std::string type;
        std::stringstream ss(line);
        ss >> type;

        if (type == "v") {
            float x, y, z; ss >> x >> y >> z;
            posx.push_back(x); posy.push_back(y); posz.push_back(z);
        }
        else if (type == "vn") {
            float x, y, z; ss >> x >> y >> z;
            nx.push_back(x); ny.push_back(y); nz.push_back(z);
        }
        else if (type == "f") {
            std::string f1, f2, f3;
            ss >> f1 >> f2 >> f3;
            std::string arr[3] = { f1, f2, f3 };

            for (int k = 0; k < 3; k++) {
                int pi = 0, ni = 0, ti = 0;
                int matched = sscanf(arr[k].c_str(), "%d//%d", &pi, &ni);
                if (matched != 2)
                    matched = sscanf(arr[k].c_str(), "%d/%d/%d", &pi, &ti, &ni);
                if (matched < 1)
                    matched = sscanf(arr[k].c_str(), "%d", &pi);

                Vertex v{};
                v.px = posx[pi - 1];
                v.py = posy[pi - 1];
                v.pz = posz[pi - 1];

                if (ni > 0) {
                    v.nx = nx[ni - 1];
                    v.ny = ny[ni - 1];
                    v.nz = nz[ni - 1];
                }
                else {
                    v.nx = 0; v.ny = 1; v.nz = 0;
                }
                mesh.push_back(v);
            }
        }
    }

    return true;
}
