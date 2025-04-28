export module Easy.Platform.Impl.OpenGL.Renderer.VertexArray;

import Easy.Core.Basic;
import Easy.Core.Util;
import Easy.Renderer.Buffer;
import Easy.Renderer.VertexArray;

namespace Easy {
    export class OpenGLVertexArray : public VertexArray {
    public:
        OpenGLVertexArray();

        virtual ~OpenGLVertexArray() override = default;

        virtual void Bind() const override;

        virtual void Unbind() const override;

        virtual void AddVertexBuffer(const Arc<VertexBuffer> &vertexBuffer) override;

        virtual void SetIndexBuffer(const Arc<IndexBuffer> &indexBuffer) override;

        [[nodiscard]] virtual const std::vector<Arc<VertexBuffer>> &GetVertexBuffers() const override {
            return m_VertexBuffers;
        }

        [[nodiscard]] virtual const Arc<IndexBuffer> &GetIndexBuffer() const override { return m_IndexBuffer; }

    private:
        struct VertexArrayDeleter {
            void operator()(uint32_t ptr) const;
        };

        // uint32_t m_RendererID;
        unique_owner_default<uint32_t, VertexArrayDeleter, 0> m_RendererID;
        uint32_t m_VertexBufferIndex = 0;
        std::vector<Arc<VertexBuffer>> m_VertexBuffers;
        Arc<IndexBuffer> m_IndexBuffer;
    };
}
