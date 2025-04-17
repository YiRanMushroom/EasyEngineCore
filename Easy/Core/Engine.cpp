module;

#include <OpenGL.pch>

module Easy.Core.Engine;

import Easy.Core.Basic;


void Easy::Engine::Run() {
    GLFWwindow *window = static_cast<GLFWwindow *>(m_EngineContext->GetNativeWindow());

    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
