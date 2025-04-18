module;

export module Easy.Core.LayerStack;
import Easy.Core.Basic;
import Easy.Core.Layer;

namespace Easy {
    export class LayerStack {
    public:
        using value_type = Arc<Layer>;
        using container_type = DynArray<value_type>;

    public:
        LayerStack() = default;

        ~LayerStack() {
            while (!m_Layers.empty()) {
                m_Layers.back()->OnDetach();
                m_Layers.pop_back();
            }
        }

        void PushLayer(value_type layer) {
            m_Layers.emplace(m_Layers.begin() + static_cast<long long>(m_OverlayStartIndex), std::move(layer));
            ++m_OverlayStartIndex;
        }

        void PushOverlay(value_type overlay) {
            m_Layers.emplace_back(std::move(overlay));
        }

        void PopLayer(const value_type &layer) {
            auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_OverlayStartIndex, layer);
            if (it != m_Layers.begin() + m_OverlayStartIndex) {
                (*it)->OnDetach();
                m_Layers.erase(it);
                --m_OverlayStartIndex;
            }
        }

        void PopOverlay(const value_type &overlay) {
            auto it = std::find(m_Layers.begin() + m_OverlayStartIndex, m_Layers.end(), overlay);
            if (it != m_Layers.end()) {
                (*it)->OnDetach();
                m_Layers.erase(it);
            }
        }

        // iterator
        [[nodiscard]] auto begin() { return m_Layers.begin(); }
        [[nodiscard]] auto end() { return m_Layers.end(); }

        // reverse iterator
        [[nodiscard]] auto rbegin() { return m_Layers.rbegin(); }
        [[nodiscard]] auto rend() { return m_Layers.rend(); }

        // const iterator
        [[nodiscard]] auto begin() const { return m_Layers.begin(); }
        [[nodiscard]] auto end() const { return m_Layers.end(); }

        // reverse const iterator
        [[nodiscard]] auto rbegin() const { return m_Layers.rbegin(); }
        [[nodiscard]] auto rend() const { return m_Layers.rend(); }

    private:
        container_type m_Layers;
        long long m_OverlayStartIndex = 0;
    };
}
