#include "Shader.h"
#include <fstream>
#include <sstream>
#include <string>

GLuint LoadShader(const char* vpath, const char* fpath) {
    auto readFile = [&](const char* path) {
        std::ifstream fs(path);
        std::stringstream ss;
        ss << fs.rdbuf();
        return ss.str();
        };

    std::string vs = readFile(vpath);
    std::string fs = readFile(fpath);

    GLuint VS = glCreateShader(GL_VERTEX_SHADER);
    const char* vsrc = vs.c_str();
    glShaderSource(VS, 1, &vsrc, nullptr);
    glCompileShader(VS);

    GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fsrc = fs.c_str();
    glShaderSource(FS, 1, &fsrc, nullptr);
    glCompileShader(FS);

    GLuint prog = glCreateProgram();
    glAttachShader(prog, VS);
    glAttachShader(prog, FS);
    glLinkProgram(prog);

    glDeleteShader(VS);
    glDeleteShader(FS);

    return prog;
}
