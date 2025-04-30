module;

#include "MacroUtils.hpp"

export module Easy.Core.Application;

export import Easy.Core.ApplicationContext;

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

        /*std::function<Arc<Window>(WindowProperties, bool, std::function<void(Event &)>)> WindowFactory;
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
        }*/
    };

    struct AppInfo {
        std::string Name;
        std::string WorkingDirectory;
        ApplicationCommandLineArgs CommandLineArgs;
        std::function<Arc<::Easy::Window>(WindowProperties, bool, std::function<void(Event &)>)> WindowFactory;
        std::function<Arc<::Easy::ImGuiLayer>()> ImGuiLayerFactory;
        bool VSync = true;
        std::optional<std::string> WindowTitle{};
        uint32_t Width = 1280;
        uint32_t Height = 720;
        bool VSyncEnabled = true;
    };

    export class Application {
    public:
        // Application(const ApplicationSpecification &specification);
        Application(AppInfo info);

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

        void Init(AppInfo info);

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
        inline static Application *s_Instance;
    };

    export class ApplicationBuilder : public Builder<ApplicationBuilder> {
        friend Builder<Application>;

    private:
        using RValueRef = ApplicationBuilder &&;

    public:
        using ValueType = Application;

        static ApplicationBuilder Start() {
            return {};
        }

        RValueRef Name(std::string name) {
            m_Name = std::move(name);
            return Next();
        }

        RValueRef WorkingDirectory(std::string workingDirectory) {
            m_WorkingDirectory = std::move(workingDirectory);
            return Next();
        }

        RValueRef CommandLineArgs(ApplicationCommandLineArgs commandLineArgs) {
            m_CommandLineArgs = std::move(commandLineArgs);
            return Next();
        }

        RValueRef WindowFactory(
            std::function<Arc<Window>(WindowProperties, bool, std::function<void(Event &)>)> windowFactory) {
            m_WindowFactory = std::move(windowFactory);
            return Next();
        }

        template<std::derived_from<Window> WindowType>
        RValueRef Window() {
            m_WindowFactory = [](WindowProperties prop, bool vSync, std::function<void(Event &)> eventCallback) {
                return Arc<::Easy::Window>{new WindowType(prop, vSync, eventCallback)};
            };
            return Next();
        }

        RValueRef ImGuiLayerFactory(std::function<Arc<ImGuiLayer>()> imguiLayerFactory) {
            m_ImGuiLayerFactory = std::move(imguiLayerFactory);
            return Next();
        }

        template<std::derived_from<ImGuiLayer> ImGuiLayerType>
        RValueRef ImGuiLayer() {
            m_ImGuiLayerFactory = []() {
                return MakeArc<ImGuiLayerType>();
            };
            return Next();
        }

        RValueRef VSync(bool vsync) {
            m_VsyncEnabled = vsync;
            return Next();
        }

        RValueRef WindowTitle(std::string title) {
            m_WindowTitle = std::move(title);
            return Next();
        }

        RValueRef WindowSize(uint32_t width, uint32_t height) {
            m_Width = width;
            m_Height = height;
            return Next();
        }

        RValueRef WindowWidth(uint32_t width) {
            m_Width = width;
            return Next();
        }

        RValueRef WindowHeight(uint32_t height) {
            m_Height = height;
            return Next();
        }

        RValueRef VSyncEnabled(bool vsyncEnabled) {
            m_VsyncEnabled = vsyncEnabled;
            return Next();
        }

    public:
        Box<ValueType> Build() noexcept(false) {
            return MakeBox<ValueType>(
                AppInfo{
                    std::move(m_Name),
                    std::move(m_WorkingDirectory),
                    std::move(m_CommandLineArgs),
                    std::move(m_WindowFactory),
                    std::move(m_ImGuiLayerFactory),
                    m_VsyncEnabled,
                    std::move(m_WindowTitle),
                    m_Width,
                    m_Height,
                    m_VsyncEnabled
                });
        }

    private:
        std::string m_Name = "EasyEngineApplication";
        std::string m_WorkingDirectory;
        ApplicationCommandLineArgs m_CommandLineArgs;
        std::function<Arc<::Easy::Window>(WindowProperties, bool, std::function<void(Event &)>)> m_WindowFactory;
        std::function<Arc<::Easy::ImGuiLayer>()> m_ImGuiLayerFactory;
        bool m_VSync = true;
        std::optional<std::string> m_WindowTitle{};
        uint32_t m_Width = 1280;
        uint32_t m_Height = 720;
        bool m_VsyncEnabled = true;
    };

    // To be defined in CLIENT
    Application *CreateApplication(ApplicationCommandLineArgs args);
}
