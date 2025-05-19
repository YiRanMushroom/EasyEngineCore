module;
#include "GLFW/glfw3.h"

export module Easy.Platform.Impl.Windows.Window;

import <Core/MacroUtils.hpp>;
import Easy.Core.Basic;
import Easy.Core.Log;
import Easy.Events.Event;
import Easy.Core.Window;
import Easy.Platform.Impl.OpenGL.Renderer.RendererAPI;

namespace Easy {
    size_t s_GLFWWindowCount = 0;

    void glfwErrorCallback(int error, const char *description) {
        EZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    export class OpenGLWindow : public Window {
    public:
        explicit OpenGLWindow(WindowProperties props, bool vSync = true,
                               std::function<void(Event &)> eventCallback = nullptr);

        virtual ~OpenGLWindow() override;

        void OnUpdate() override;

        [[nodiscard]] uint32_t GetWidth() const override;

        [[nodiscard]] uint32_t GetHeight() const override;

        void SetVSync(bool enabled) override;

        [[nodiscard]] bool IsVSync() const override;

        void SetEventCallback(std::function<void(Event &)> callback) override;

        [[nodiscard]] GLFWwindow *GetNativeWindow() const override;

        OpenGLWindow(const OpenGLWindow &) = delete;

        OpenGLWindow &operator=(const OpenGLWindow &) = delete;

    private:
        std::string Title;
        int Width;
        int Height;
        bool VSync;
        std::move_only_function<void(Event &)> EventCallback;
        GLFWwindow *m_Window;

        void Init();
    };
}
