module;

#include <OpenGL.hpp>
#include "Core/MacroUtils.hpp"

export module Easy.Platform.WindowsWindow;

import Easy.Core.Basic;
import Easy.Events.Event;
import Easy.Core.Window;

namespace Easy {
    size_t g_GLFWWindowCount = 0;

    class WindowsWindow : public Window {
    public:
        explicit WindowsWindow(WindowProperties props);

        virtual ~WindowsWindow() override;

        void OnUpdate() override;

        [[nodiscard]] uint32_t GetWidth() const override;

        [[nodiscard]] uint32_t GetHeight() const override;

        void SetVSync(bool enabled) override;

        [[nodiscard]] bool IsVSync() const override;

        void SetEventCallback(std::move_only_function<void(Event &)> callback) override;

        [[nodiscard]] void *GetNativeWindow() const override;

    private:
        struct WindowData {
            std::string Title;
            int Width;
            int Height;
            bool VSync;
            std::move_only_function<void(Event &)> EventCallback;
        };

        WindowData m_Data;

        GLFWwindow *m_Window;
    };

    WindowsWindow::WindowsWindow(WindowProperties properties)
        : m_Data{properties.Title, properties.Width, properties.Height, false, nullptr} {

    }

    WindowsWindow::~WindowsWindow() {}
    void WindowsWindow::OnUpdate() {}
    uint32_t WindowsWindow::GetWidth() const {}
    uint32_t WindowsWindow::GetHeight() const {}
    void WindowsWindow::SetVSync(bool enabled) {}
    bool WindowsWindow::IsVSync() const {}
    void WindowsWindow::SetEventCallback(std::move_only_function<void(Event &)> callback) {}
    void *WindowsWindow::GetNativeWindow() const {}
}
