#define _CRT_SECURE_NO_WARNINGS
#include "OBJLoader.h"
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

bool LoadOBJ(const char* path, vector<Vertex>& mesh)
{
    vector<float> px, py, pz;
    vector<float> nx, ny, nz;

    ifstream file(path);
    if (!file.is_open()) return false;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string type;
        ss >> type;

        if (type == "v") {
            float x, y, z; ss >> x >> y >> z;
            px.push_back(x); py.push_back(y); pz.push_back(z);
        }
        else if (type == "vn") {
            float x, y, z; ss >> x >> y >> z;
            nx.push_back(x); ny.push_back(y); nz.push_back(z);
        }
        else if (type == "f") {
            string f1, f2, f3; ss >> f1 >> f2 >> f3;
            string tri[3] = { f1, f2, f3 };

            for (int i = 0; i < 3; i++) {
                int pi, ni;
                sscanf(tri[i].c_str(), "%d//%d", &pi, &ni);

                Vertex v;
                v.px = px[pi - 1];
                v.py = py[pi - 1];
                v.pz = pz[pi - 1];

                v.nx = nx[ni - 1];
                v.ny = ny[ni - 1];
                v.nz = nz[ni - 1];

                mesh.push_back(v);
            }
        }
    }
    return true;
}
