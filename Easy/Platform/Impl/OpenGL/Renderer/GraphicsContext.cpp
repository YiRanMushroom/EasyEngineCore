module;

#include <OpenGL.hpp>

module Easy.Platform.Impl.OpenGL.Renderer.GraphicsContext;

import <Core/MacroUtils.hpp>;
import Easy.Core.Log;

namespace Easy {

    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle)
    {
        EZ_CORE_ASSERT(windowHandle, "Window handle is null!");
    }

    void OpenGLContext::Init()
    {
        // EZ_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        EZ_CORE_ASSERT(status, "Failed to initialize Glad!");

        EZ_CORE_INFO("OpenGL Info:");
        EZ_CORE_INFO("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        EZ_CORE_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        EZ_CORE_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

        EZ_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Hazel requires at least OpenGL version 4.5!");
    }

    void OpenGLContext::SwapBuffers()
    {
        // EZ_PROFILE_FUNCTION();

        glfwSwapBuffers(m_WindowHandle);
    }

}