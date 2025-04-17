module;

#include <OpenGL.pch>

module Easy.Core.Engine;

import Easy.Core.Basic;


void Easy::Engine::run_test() {
    GLFWwindow* window;
    if (!glfwInit())
        return;

    // cout << "default shader lang: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    // select opengl version
    // int major, minor, rev;
    // glfwGetVersion(&major, &minor, &rev);
    // cout << "glfw major.minor " << major << "." << minor << "." << rev << endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    std::cout << "OpenGL shader language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}