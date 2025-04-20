module;

#include "MacroUtils.hpp"

export module Easy.Core.Application;

import Easy.Core.Basic;
import Easy.Core.Window;
import Easy.Core.Layer;
import Easy.Core.Util;
import Easy.Events.Event;
import Easy.ImGui.ImGuiLayer;
import Easy.Events.ApplicationEvent;
import Easy.Core.LayerStack;

namespace Easy {
    export struct ApplicationCommandLineArgs {
        int Count = 0;
        char **Args = nullptr;

        const char *operator[](int index) const {
            EZ_CORE_ASSERT(index < Count);
            return Args[index];
        }
    };

    export struct ApplicationSpecification {
        std::string Name = "Hazel Application";
        std::string WorkingDirectory;
        ApplicationCommandLineArgs CommandLineArgs;

        std::function<Arc<Window>(WindowProperties, bool, std::function<void(Event &)>)> WindowFactory;
        std::function<Arc<ImGuiLayer>()> ImGuiLayerFactory;

        template<typename WindowType, typename ImGuiLayerType>
        static ApplicationSpecification Make(std::string name, std::string workingDirectory = "",
                                             ApplicationCommandLineArgs commandLineArgs = {}) {
            return {
                std::move(name), std::move(workingDirectory), std::move(commandLineArgs),
                [](WindowProperties prop, bool vSync, std::function<void(Event &)> eventCallback) {
                    return Arc<Window>{new WindowType(prop, vSync, eventCallback)};
                },
                []() {
                    return MakeArc<ImGuiLayerType>();
                }
            };
        }
    };

    export class Application {
    public:
        Application(const ApplicationSpecification &specification);

        virtual ~Application();

        void OnEvent(Event &e);

        void PushLayer(Arc<Layer> layer);

        void PushOverlay(Arc<Layer> layer);

        Window &GetWindow() { return *m_Window; }

        void Close();

        Arc<ImGuiLayer> GetImGuiLayer() { return m_ImGuiLayer; }

        static Application &Get() { return *s_Instance; }

        const ApplicationSpecification &GetSpecification() const { return m_Specification; }

        void SubmitToMainThread(const std::function<void()> &function);

        // private:
        void Run();

        bool OnWindowClose(WindowCloseEvent &e);

        bool OnWindowResize(WindowResizeEvent &e);

        void ExecuteMainThreadQueue();

    private:
        ApplicationSpecification m_Specification;
        Arc<Window> m_Window;
        Arc<ImGuiLayer> m_ImGuiLayer;
        bool m_Running = true;
        bool m_Minimized = false;
        LayerStack m_LayerStack;
        SteadyFrameTimer m_FrameTimer;

        std::vector<std::function<void()>> m_MainThreadQueue;
        std::mutex m_MainThreadQueueMutex;

    private:
        static Application *s_Instance;
    };

    // To be defined in CLIENT
    Application *CreateApplication(ApplicationCommandLineArgs args);
}
