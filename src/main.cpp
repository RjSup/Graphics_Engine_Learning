#include <iostream>
#include <GL/glew.h> // first - handles loading extensions and includes the OpenGL headers
#include <GLFW/glfw3.h> // second

int main()
{
    // init GLFW before to get OPENGL context
    if (!glfwInit())
        return -1;

    // create a window context
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // create OpenGL context
    glfwMakeContextCurrent(window);

    // check if GLEW working after GLFW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
    } else {
        // if working show GPU information
        std::cout << glGetString(GL_VERSION) << std::endl;
    }

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
