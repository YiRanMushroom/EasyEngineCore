module;

#include <OpenGL.pch>

module Easy.Platform.OpenGLEngineContext;

import Easy.Core.Context;
import Easy.Core.Basic;

namespace Easy {
    void OpenGLEngineContext::Init() {
        GLFWwindow *window;
        if (!glfwInit())
            return;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
        if (!window) {
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            glfwDestroyWindow(window);
            glfwTerminate();
            return;
        }

        m_Window = window;
    }

    void OpenGLEngineContext::Destroy() {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }
}
