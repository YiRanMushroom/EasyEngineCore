module;

#include "MacroUtils.hpp"
#include <glad/glad.h>

module Easy.Core.Application;

import Easy.Core.Basic;
import Easy.Core.Window;
import Easy.Core.Layer;
import Easy.Core.LayerStack;
import Easy.Core.Util;
import Easy.Events.Event;
import Easy.Events.ApplicationEvents;
import Easy.ImGui.ImGuiLayer;
import Easy.Renderer.Renderer;
import Easy.Scripting;

namespace Easy {
    Application::Application(AppInfo info) : m_Specification{
        std::move(info.Name), std::move(info.WorkingDirectory), std::move(info.CommandLineArgs)
    } {
        if (s_Instance) {
            EZ_CORE_CRITICAL("Application already exists!");
        }

        s_Instance = this;

        Init(std::move(info));
    }

    void Application::Init(AppInfo info) {
        // ScriptEngine::Init();
        Log::Init();

        ScriptingEngine::Init();

        if (!m_Specification.WorkingDirectory.empty())
            std::filesystem::current_path(m_Specification.WorkingDirectory);

        m_Window = info.WindowFactory(WindowProperties(
                                          info.WindowTitle ? info.WindowTitle.value() : m_Specification.Name,
                                          static_cast<int>(info.Width),
                                          static_cast<int>(info.Height)), info.VSyncEnabled,
                                      std::bind(&Application::OnEvent, this, std::placeholders::_1));

        Renderer::Init();

        m_ImGuiLayer = info.ImGuiLayerFactory();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application() {
        m_LayerStack.clear();
        Renderer::Shutdown();
        ScriptingEngine::Shutdown();
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
        // dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));
        // dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

        dispatcher.Dispatch(&Application::OnWindowResize, this);
        dispatcher.Dispatch(&Application::OnWindowClose, this);

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
            if (e.Handled)
                break;
            (*it)->OnEvent(e);
        }
    }

    void Application::Run() {
        while (m_Running) {
            EZ_PROFILE_FRAME_START();
            float timestep = m_FrameTimer.GetDeltaTime().count();

            ExecuteMainThreadQueue();

            if (!m_Minimized) {
                {
                    // HZ_PROFILE_SCOPE("LayerStack OnUpdate");

                    for (const auto &layer: m_LayerStack)
                        layer->OnUpdate(timestep);
                }

                m_ImGuiLayer->Begin(); {
                    for (const auto &layer: m_LayerStack)
                        layer->OnImGuiRender();
                }
                m_ImGuiLayer->End();
            }

            m_Window->OnUpdate();
            EZ_PROFILE_FRAME_END();
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
        glViewport(0, 0, e.GetWidth(), e.GetHeight());

        return false;
    }

    void Application::ExecuteMainThreadQueue() {
        std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);

        for (auto &func: m_MainThreadQueue)
            func();

        m_MainThreadQueue.clear();
    }
}
