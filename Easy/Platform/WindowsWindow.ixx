module;

#include <OpenGL.hpp>
#include "Core/MacroUtils.hpp"

export module Easy.Platform.WindowsWindow;

import Easy.Events.Event;
import Easy.Core.Window;

namespace Easy {
    size_t s_GLFWWindowCount = 0;

    void glfwErrorCallback(int error, const char *description) {
        EZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    class WindowsWindow : public Window {
    public:
        explicit WindowsWindow(WindowProperties props, bool vSync = true,
                               std::move_only_function<void(Event &)> eventCallback = nullptr);

        virtual ~WindowsWindow() override;

        void OnUpdate() override;

        [[nodiscard]] uint32_t GetWidth() const override;

        [[nodiscard]] uint32_t GetHeight() const override;

        void SetVSync(bool enabled) override;

        [[nodiscard]] bool IsVSync() const override;

        void SetEventCallback(std::move_only_function<void(Event &)> callback) override;

        [[nodiscard]] void *GetNativeWindow() const override;

        WindowsWindow(const WindowsWindow &) = delete;

        WindowsWindow &operator=(const WindowsWindow &) = delete;

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
