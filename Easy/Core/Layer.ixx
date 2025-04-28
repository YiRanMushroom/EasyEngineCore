module;

export module Easy.Core.Layer;

import Easy.Core.Basic;
import Easy.Events.Event;

namespace Easy {
    export class Layer {
    public:
        Layer(const std::string &name = "Layer") : m_DebugName(name) {}

        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(float) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event &) {}

        const std::string &GetName() const { return m_DebugName; }

    protected:
        std::string m_DebugName;
    };
}
