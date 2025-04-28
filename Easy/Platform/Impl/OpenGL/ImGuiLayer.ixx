export module Easy.Platform.Impl.OpenGL.ImGuiLayer;
import Easy.Core.KeyCodes;
import Easy.ImGui.ImGuiLayer;
import Easy.Events.Event;

namespace Easy {
    export class OpenGLImGuiLayer : public ImGuiLayer {
    public:
        OpenGLImGuiLayer();

        virtual ~OpenGLImGuiLayer() override;

        virtual void OnAttach() override;

        virtual void OnDetach() override;

        virtual void OnEvent(Event &e) override;

        virtual void Begin() override;

        virtual void End() override;

        virtual void BlockEvents(bool block) override;

        virtual void SetDarkThemeColors() override;

        [[nodiscard]] virtual uint32_t GetActiveWidgetID() const override;

        virtual void SetToggleImGuiBlockEventsKey(Key::KeyCode key) override;

        virtual void SetTemporaryUnblockEventsKey(Key::KeyCode key) override;

    private:
        Key::KeyCode m_ToggleImGuiBlockEventsKey = Key::F5;
        Key::KeyCode m_TemporaryUnblockEventsKey = Key::LeftControl;

        bool m_BlockEvents = true;
    };
}
