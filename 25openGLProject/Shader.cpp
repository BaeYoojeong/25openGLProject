#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

static std::string ReadFile(const char* path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Cannot open shader: " << path << std::endl;
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

static GLuint Compile(GLenum type, const char* src) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, NULL);
    glCompileShader(s);

    GLint ok;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);

    if (!ok) {
        char log[2048];
        glGetShaderInfoLog(s, 2048, NULL, log);
        std::cerr << "Shader compile error:\n" << log << std::endl;
    }
    return s;
}

GLuint LoadShader(const char* vpath, const char* fpath) {
    std::string v = ReadFile(vpath);
    std::string f = ReadFile(fpath);

    GLuint vs = Compile(GL_VERTEX_SHADER, v.c_str());
    GLuint fs = Compile(GL_FRAGMENT_SHADER, f.c_str());

    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);

    glLinkProgram(prog);

    GLint ok;
    glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[2048];
        glGetProgramInfoLog(prog, 2048, NULL, log);
        std::cerr << "Link error:\n" << log << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return prog;
}
