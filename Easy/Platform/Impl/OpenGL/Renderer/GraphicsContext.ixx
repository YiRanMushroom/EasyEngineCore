export module Easy.Platform.Impl.OpenGL.Renderer.GraphicsContext;

import "Platform/Impl/OpenGL/GLFW_OpenGL.hpp";
import Easy.Core.Basic;
import Easy.Renderer.GraphicsContext;

namespace Easy {
    export class OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext(GLFWwindow* windowHandle);

        virtual void Init() override;
        virtual void SwapBuffers() override;
    private:
        GLFWwindow* m_WindowHandle;
    };
}