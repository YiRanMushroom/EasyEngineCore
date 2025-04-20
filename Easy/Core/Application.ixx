module;

#include "MacroUtils.hpp"

export module Easy.Core.Application;

import Easy.Core.Basic;
import Easy.Core.Window;
import Easy.Core.Layer;
import Easy.Events.Event;
import Easy.ImGui.ImGuiLayer;
import Easy.Events.ApplicationEvent;
import Easy.Core.LayerStack;

namespace Easy {

    export struct ApplicationCommandLineArgs
    {
        int Count = 0;
        char** Args = nullptr;

        const char* operator[](int index) const
        {
            EZ_CORE_ASSERT(index < Count);
            return Args[index];
        }
    };

    export struct ApplicationSpecification
    {
        std::string Name = "Hazel Application";
        std::string WorkingDirectory;
        ApplicationCommandLineArgs CommandLineArgs;
    };

    export class Application
    {
    public:
        Application(const ApplicationSpecification& specification);
        virtual ~Application();

        void OnEvent(Event& e);

        void PushLayer(Arc<Layer> layer);
        void PushOverlay(Arc<Layer> layer);

        Window& GetWindow() { return *m_Window; }

        void Close();

        Arc<ImGuiLayer> GetImGuiLayer() { return m_ImGuiLayer; }

        static Application& Get() { return *s_Instance; }

        const ApplicationSpecification& GetSpecification() const { return m_Specification; }

        void SubmitToMainThread(const std::function<void()>& function);
    // private:
        void Run();
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

        void ExecuteMainThreadQueue();
    private:
        ApplicationSpecification m_Specification;
        Arc<Window> m_Window;
        Arc<ImGuiLayer> m_ImGuiLayer;
        bool m_Running = true;
        bool m_Minimized = false;
        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;

        std::vector<std::function<void()>> m_MainThreadQueue;
        std::mutex m_MainThreadQueueMutex;
    private:
        static Application* s_Instance;
        // friend int ::main(int argc, char** argv);
    };

    // To be defined in CLIENT
    Application* CreateApplication(ApplicationCommandLineArgs args);
}