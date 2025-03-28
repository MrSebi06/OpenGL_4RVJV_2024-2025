#include "app.h"

#include <cassert>
#include <GLFW/glfw3.h>

#ifdef WIN32
#include <GL/wglew.h>
#endif


bool Application::Initialize(const int width, const int height) {
    assert(glewInit() == GLEW_NO_ERROR);

    this->width = width;
    this->height = height;

    m_basicShader.LoadVertexShader("../shader/basic.vs.glsl");
    m_basicShader.LoadFragmentShader("../shader/basic.fs.glsl");
    m_basicShader.Create();

#ifdef WIN32
    // Enable VSync
    wglSwapIntervalEXT(1);
#endif

    glClearColor(0.5f, 0.5f, 0.5f, 1.f);

    static const Vertex triangle[] = {
        Vertex(Vec2(-0.5f, -0.5f), Vec3(1.0f, 0.0f, 0.0f)),
        Vertex(Vec2(0.5f, -0.5f), Vec3(0.0f, 1.0f, 0.0f)),
        Vertex(Vec2(0.0f, 0.5f), Vec3(0.0f, 0.0f, 1.0f))
    };

    // glGenVertexArrays(1, &VAO);
    // glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 3, triangle, GL_STATIC_DRAW);

    const auto basicProgram = m_basicShader.GetProgram();
    glUseProgram(basicProgram);

    constexpr int stride = sizeof(Vertex);
    const int loc_position = glGetAttribLocation(basicProgram, "a_Position");
    const int loc_color = glGetAttribLocation(basicProgram, "a_Color");
    glEnableVertexAttribArray(loc_position);
    glEnableVertexAttribArray(loc_color);
    glVertexAttribPointer(loc_position, 2, GL_FLOAT, GL_FALSE, stride, nullptr);
    glVertexAttribPointer(loc_color, 3, GL_FLOAT, GL_FALSE, stride,
                          reinterpret_cast<const void *>(offsetof(Vertex, color)));

    // glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return true;
}


void Application::Terminate() const {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}


void Application::Render() const {
    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT);

    // glDrawArrays(GL_TRIANGLES, 0, 3);
    // glBindVertexArray(VAO);
    constexpr unsigned int indices[] = {0, 1, 2};
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, indices);
    // glBindVertexArray(0);
}
