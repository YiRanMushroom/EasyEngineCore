module;

#include <OpenGL.hpp>
#include "Core/MacroUtils.hpp"

module Easy.Platform.WindowsWindow;

import Easy.Core.Basic;
import Easy.Core.KeyCodes;
import Easy.Events.Event;
import Easy.Events.KeyEvents;
import Easy.Events.MouseEvents;
import Easy.Events.ApplicationEvent;
import Easy.Core.Window;

namespace Easy {
    WindowsWindow::WindowsWindow(WindowProperties properties, bool vSync,
                                 std::move_only_function<void(Event &)> eventCallback)
        : Title(std::move(properties.Title)), Width(properties.Width), Height(properties.Height),
          VSync(vSync), EventCallback(std::move(eventCallback)) {
        Init();
    }

    WindowsWindow::~WindowsWindow() {
        glfwDestroyWindow(m_Window);

        --s_GLFWWindowCount;
        if (s_GLFWWindowCount == 0) {
            glfwTerminate();
        }
    }

    void WindowsWindow::OnUpdate() {
        glfwPollEvents();

        glfwSwapBuffers(m_Window);
    }

    uint32_t WindowsWindow::GetWidth() const {
        return Width;
    }

    uint32_t WindowsWindow::GetHeight() const {
        return Height;
    }

    void WindowsWindow::SetVSync(bool enabled) {
        if (enabled) {
            glfwSwapInterval(1);
        } else {
            glfwSwapInterval(0);
        }
    }

    bool WindowsWindow::IsVSync() const {
        return VSync;
    }

    void WindowsWindow::SetEventCallback(std::move_only_function<void(Event &)> callback) {}

    void *WindowsWindow::GetNativeWindow() const {
        return m_Window;
    }

    void WindowsWindow::Init() {
        EZ_CORE_INFO("Creating window {0} ({1}, {2})", Title, Width, Height);

        if (s_GLFWWindowCount == 0) {
            if (!glfwInit()) {
                EZ_CORE_ERROR("Failed to initialize GLFW");
                return;
            }

            glfwSetErrorCallback(glfwErrorCallback);
        }
        // Inner Scope
        {
#ifdef EZ_DEBUG
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            m_Window = glfwCreateWindow(Width, Height, Title.c_str(), nullptr, nullptr);
            if (!m_Window) {
                EZ_CORE_ERROR("Failed to create GLFW window");
                glfwTerminate();
                return;
            }
            ++s_GLFWWindowCount;
        }

        glfwMakeContextCurrent(m_Window);

        SetVSync(VSync);

        // Initialize Glad
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            EZ_CORE_ERROR("Failed to initialize Glad");
            glfwDestroyWindow(m_Window);
            glfwTerminate();
        }

        glfwSetWindowUserPointer(m_Window, this);

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height) {
            auto *win = static_cast<WindowsWindow *>(glfwGetWindowUserPointer(window));
            win->Width = width;
            win->Height = height;
            WindowResizeEvent event(width, height);
            win->EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window) {
            auto *win = static_cast<WindowsWindow *>(glfwGetWindowUserPointer(window));
            WindowCloseEvent event;
            win->EventCallback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            auto *win = static_cast<WindowsWindow *>(glfwGetWindowUserPointer(window));
            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent event(key, 0);



                    win->EventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent event(key);
                    win->EventCallback(event);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyPressedEvent event(key, true);
                    win->EventCallback(event);
                    break;
                }
                default: EZ_CORE_ERROR("Unknown key action: {0}", action);
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow *window, unsigned int keycode) {
            auto *win = static_cast<WindowsWindow *>(glfwGetWindowUserPointer(window));
            KeyTypedEvent event(keycode);
            win->EventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods) {
            auto *win = static_cast<WindowsWindow *>(glfwGetWindowUserPointer(window));
            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent event(button);
                    win->EventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent event(button);
                    win->EventCallback(event);
                    break;
                }
                default: EZ_CORE_ERROR("Unknown mouse button action: {0}", action);
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xOffset, double yOffset) {
            auto *win = static_cast<WindowsWindow *>(glfwGetWindowUserPointer(window));
            MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
            win->EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xPos, double yPos) {
            auto *win = static_cast<WindowsWindow *>(glfwGetWindowUserPointer(window));
            MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
            win->EventCallback(event);
        });
    }
}
