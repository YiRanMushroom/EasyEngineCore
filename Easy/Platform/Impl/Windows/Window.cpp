module;

#include <OpenGL.hpp>
#include "Core/MacroUtils.hpp"

module Easy.Platform.Impl.Windows.Window;

import Easy.Core.Basic;
import Easy.Core.KeyCodes;
import Easy.Core.ApplicationContext;
import Easy.Core.Input;
import Easy.Events.Event;
import Easy.Events.KeyEvents;
import Easy.Events.MouseEvents;
import Easy.Events.ApplicationEvents;
import Easy.Core.Window;
import Easy.Renderer.RendererAPI;

namespace Easy {
    OpenGLWindow::OpenGLWindow(WindowProperties properties, bool vSync,
                               std::function<void(Event &)> eventCallback)
        : Title(std::move(properties.Title)), Width(properties.Width), Height(properties.Height),
          VSync(vSync), EventCallback(std::move(eventCallback)) {
        Init();
    }

    OpenGLWindow::~OpenGLWindow() {
        ApplicationContext::ApplicationRendererAPI = RendererAPI::API::None;

        Input::Shutdown();

        glfwDestroyWindow(m_Window);

        --s_GLFWWindowCount;
        if (s_GLFWWindowCount == 0) {
            glfwTerminate();
        }
    }

    void OpenGLWindow::OnUpdate() {
        glfwPollEvents();

        glfwSwapBuffers(m_Window);
    }

    uint32_t OpenGLWindow::GetWidth() const {
        return Width;
    }

    uint32_t OpenGLWindow::GetHeight() const {
        return Height;
    }

    void OpenGLWindow::SetVSync(bool enabled) {
        if (enabled) {
            glfwSwapInterval(1);
        } else {
            glfwSwapInterval(0);
        }
    }

    bool OpenGLWindow::IsVSync() const {
        return VSync;
    }

    void OpenGLWindow::SetEventCallback(std::function<void(Event &)> callback) {
        EventCallback = std::move(callback);
    }

    GLFWwindow *OpenGLWindow::GetNativeWindow() const {
        return m_Window;
    }

    void OpenGLWindow::Init() {
        WindowAPI::s_API = WindowAPI::API::Windows;

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

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
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

        Input::Init();

        SetVSync(VSync);

        ApplicationContext::ApplicationRendererAPI = RendererAPI::API::OpenGL;

        // SetRendererAPI(RendererAPI::API::OpenGL);

        glfwSetWindowUserPointer(m_Window, this);

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height) {
            auto *win = static_cast<OpenGLWindow *>(glfwGetWindowUserPointer(window));
            win->Width = width;
            win->Height = height;
            WindowResizeEvent event(width, height);
            win->EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window) {
            auto *win = static_cast<OpenGLWindow *>(glfwGetWindowUserPointer(window));
            WindowCloseEvent event;
            win->EventCallback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            auto *win = static_cast<OpenGLWindow *>(glfwGetWindowUserPointer(window));
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
            auto *win = static_cast<OpenGLWindow *>(glfwGetWindowUserPointer(window));
            KeyTypedEvent event(keycode);
            win->EventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods) {
            auto *win = static_cast<OpenGLWindow *>(glfwGetWindowUserPointer(window));
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
            auto *win = static_cast<OpenGLWindow *>(glfwGetWindowUserPointer(window));
            MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
            win->EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xPos, double yPos) {
            auto *win = static_cast<OpenGLWindow *>(glfwGetWindowUserPointer(window));
            MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
            win->EventCallback(event);
        });
    }
}
