module;

export module Easy.ImGui.ImGuiLayer;
import Easy.Core.Layer;
import Easy.Events.Event;

namespace Easy {
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        virtual ~ImGuiLayer() override = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event& e) override;

        void Begin();
        void End();

        void BlockEvents(bool block) { m_BlockEvents = block; }

        void SetDarkThemeColors();

        uint32_t GetActiveWidgetID() const;
    private:
        bool m_BlockEvents = true;
    };
}