module;

#include "MacroUtils.hpp"
#include "GLFW/glfw3.h"

module Easy.Core.Application;

import Easy.Core.Basic;
import Easy.Core.Window;
import Easy.Core.Layer;
import Easy.Events.Event;
import Easy.ImGui.ImGuiLayer;
import Easy.Events.ApplicationEvent;
import Easy.Core.LayerStack;
import Easy.Platform.WindowsWindow;

namespace Easy {
    Application *Application::s_Instance = nullptr;

    Application::Application(const ApplicationSpecification &specification)
        : m_Specification(specification) {
        EZ_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        Log::Init();

        // Set working directory here
        if (!m_Specification.WorkingDirectory.empty())
            std::filesystem::current_path(m_Specification.WorkingDirectory);

        // m_Window = Window::Create(WindowProps(m_Specification.Name));
        m_Window = Arc<Window>{
            new WindowsWindow(WindowProperties(m_Specification.Name), true,
                              std::bind(&Application::OnEvent, this, std::placeholders::_1)
            )
        };
        // m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));

        // Renderer::Init();

        m_ImGuiLayer = Arc<ImGuiLayer>{new ImGuiLayer()};
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application() {
        // ScriptEngine::Shutdown();
        // Renderer::Shutdown();
    }

    void Application::PushLayer(Arc<Layer> layer) {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Arc<Layer> layer) {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::Close() {
        m_Running = false;
    }

    void Application::SubmitToMainThread(const std::function<void()> &function) {
        std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);

        m_MainThreadQueue.emplace_back(function);
    }

    void Application::OnEvent(Event &e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));
        dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
            if (e.Handled)
                break;
            (*it)->OnEvent(e);
        }
    }

    void Application::Run() {
        while (m_Running) {
            float time = glfwGetTime();
            float timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            ExecuteMainThreadQueue();

            if (!m_Minimized) {
                {
                    // HZ_PROFILE_SCOPE("LayerStack OnUpdate");

                    for (const auto& layer: m_LayerStack)
                        layer->OnUpdate(timestep);
                }

                m_ImGuiLayer->Begin();
                {

                    for (const auto& layer: m_LayerStack)
                        layer->OnImGuiRender();
                }
                m_ImGuiLayer->End();
            }

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent &e) {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent &e) {
        if (e.GetWidth() == 0 || e.GetHeight() == 0) {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        // Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

        return false;
    }

    void Application::ExecuteMainThreadQueue() {
        std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);

        for (auto &func: m_MainThreadQueue)
            func();

        m_MainThreadQueue.clear();
    }
}
