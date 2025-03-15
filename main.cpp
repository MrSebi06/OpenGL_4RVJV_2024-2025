#include <cassert>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef WIN32
#include <GL/wglew.h>
#endif

#include "GLShader.h"


struct Application {
    GLShader m_basicShader;
    GLuint VBO;

    int width{};
    int height{};

    bool Initialize(int width, int height);

    void Terminate() const;

    void Render();
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


bool Application::Initialize(const int width, const int height) {
    this->width = width;
    this->height = height;

    const GLenum glewOk = glewInit();
    assert(glewOk == GLEW_NO_ERROR);

    m_basicShader.LoadVertexShader("shader/basic.vs.glsl");
    m_basicShader.LoadFragmentShader("shader/basic.fs.glsl");
    m_basicShader.Create();

#ifdef WIN32
    // Enable VSync
    wglSwapIntervalEXT(1);
#endif

    static const Vertex triangle[] = {
        Vertex(Vec2(-0.5f, -0.5f), Vec3(1.0f, 0.0f, 0.0f)),
        Vertex(Vec2(0.5f, -0.5f), Vec3(0.0f, 1.0f, 0.0f)),
        Vertex(Vec2(0.0f, 0.5f), Vec3(0.0f, 0.0f, 1.0f))
    };

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 3, triangle, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
}

void Application::Terminate() const {
    glDeleteBuffers(1, &VBO);
}

void Application::Render() {
    glViewport(0, 0, width, height);

    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    const auto basicProgram = m_basicShader.GetProgram();
    glUseProgram(basicProgram);


    const int loc_position = glGetAttribLocation(basicProgram, "a_Position");
    const int loc_color = glGetAttribLocation(basicProgram, "a_Color");
    constexpr int stride = sizeof(Vertex);
    glEnableVertexAttribArray(loc_position);
    glEnableVertexAttribArray(loc_color);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(loc_position, 2, GL_FLOAT, GL_FALSE, stride, nullptr);
    glVertexAttribPointer(loc_color, 3, GL_FLOAT, GL_FALSE, stride,
                          reinterpret_cast<const void *>(offsetof(Vertex, color)));

    glDrawArrays(GL_TRIANGLES, 0, 3);
}


int main() {
    Application app;
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow *window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    app.Initialize(640, 480);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        app.Render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    app.Terminate();

    return 0;
}
