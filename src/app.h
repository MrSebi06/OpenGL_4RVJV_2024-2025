#ifndef APP_H
#define APP_H

#define GLEW_STATIC
#include <GL/glew.h>

#include "GLShader.h"

struct Application {
private:
    GLShader m_basicShader;
    GLuint VBO;

    int width{};
    int height{};

public:
    Application(): VBO(0) {
    }

    Application(const GLShader &m_basic_shader, const GLuint vbo, const int width, const int height)
        : m_basicShader(m_basic_shader),
          VBO(vbo),
          width(width),
          height(height) {
    }

    bool Initialize(int width, int height);

    void Terminate() const;

    void Render() const;
};

struct Vec2 {
    float x;
    float y;
};

struct Vec3 {
    float x;
    float y;
    float z;
};

struct Vertex {
    Vec2 position;
    Vec3 color;

    Vertex(const Vec2 position, const Vec3 color): position(position), color(color) {
    }
};

#endif //APP_H
