module;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Core/MacroUtils.hpp>

module Easy.Platform.Impl.OpenGL.Renderer.RendererAPI;

import Easy.Core.Application;
import Easy.Renderer.RendererAPI;
import Easy.Renderer.Shader;
import Easy.Renderer.VertexArray;
import Easy.Core.Basic;
import Easy.Vendor.glm;
import Easy.Platform.Impl.OpenGL.Renderer.Shader;

namespace Easy {
    void OpenGLMessageCallback(
        unsigned source,
        unsigned type,
        unsigned id,
        unsigned severity,
        int length,
        const char *message,
        const void *userParam) {
        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH: EZ_CORE_CRITICAL(message);
                return;
            case GL_DEBUG_SEVERITY_MEDIUM: EZ_CORE_ERROR(message);
                return;
            case GL_DEBUG_SEVERITY_LOW: EZ_CORE_WARN(message);
                return;
            case GL_DEBUG_SEVERITY_NOTIFICATION: EZ_CORE_TRACE(message);
                return;
        }

        EZ_CORE_ASSERT(false, "Unknown severity level!");
    }

    OpenGLRendererAPI::OpenGLRendererAPI() {
        s_CurrentRendererAPI = this;
    }

    OpenGLRendererAPI::~OpenGLRendererAPI() {
        s_CurrentRendererAPI = nullptr;
    }

    void OpenGLRendererAPI::Init() {
        // Initialize Glad
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            EZ_CORE_ERROR("Failed to initialize Glad");
            glfwDestroyWindow(static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow()));
            glfwTerminate();
        }

        // EZ_PROFILE_FUNCTION();

#ifdef EZ_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGLMessageCallback, nullptr);

        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LINE_SMOOTH);
    }

    void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4 &color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(const Arc<VertexArray> &vertexArray, uint32_t indexCount) {
        vertexArray->Bind();
        uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLRendererAPI::DrawLines(const Arc<VertexArray> &vertexArray, uint32_t vertexCount) {
        vertexArray->Bind();
        glDrawArrays(GL_LINES, 0, vertexCount);
    }

    void OpenGLRendererAPI::SetLineWidth(float width) {
        glLineWidth(width);
    }

    Arc<Shader> OpenGLRendererAPI::CreateShader(const std::string &name, const std::string &vertexSrc,
                                                const std::string &fragmentSrc) {
        return MakeArc<OpenGLShader>(name, vertexSrc, fragmentSrc);
    }

    Arc<Shader> OpenGLRendererAPI::CreateShader(const std::string &filepath) {
        return MakeArc<OpenGLShader>(filepath);
    }
}
