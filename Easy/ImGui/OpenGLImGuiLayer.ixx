module;

export module Easy.ImGui.ImGuiLayer;

import Easy.Core.Basic;
import Easy.Core.KeyCodes;
import Easy.Core.Layer;
import Easy.Events.Event;

namespace Easy {
    export class ImGuiLayer : public Layer {
    public:
        explicit ImGuiLayer(const std::string &name = "ImGuiLayer") : Layer(name) {}

        virtual ~ImGuiLayer() override = default;

        virtual void OnAttach() override = 0;

        virtual void OnDetach() override = 0;

        virtual void OnEvent(Event &e) override = 0;

        virtual void Begin() = 0;

        virtual void End() = 0;

        virtual void BlockEvents(bool block) = 0;

        virtual void SetDarkThemeColors() = 0;

        [[nodiscard]] virtual uint32_t GetActiveWidgetID() const = 0;

        virtual void SetToggleImGuiBlockEventsKey(Key::KeyCode key) = 0;

        virtual void SetTemporaryUnblockEventsKey(Key::KeyCode key) = 0;
    };
}
