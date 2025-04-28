export module Easy.Platform.Impl.OpenGL.Renderer.Buffer;

import Easy.Renderer.Buffer;
import easy.vendor.glm;
import Easy.Core.Util;

namespace Easy {
    export class OpenGLVertexBuffer : public VertexBuffer {
    public:
        explicit OpenGLVertexBuffer(uint32_t size);

        OpenGLVertexBuffer(const float *vertices, uint32_t size);

        virtual ~OpenGLVertexBuffer() override = default;

        virtual void Bind() const override;

        virtual void Unbind() const override;

        virtual void SetData(const void *data, uint32_t size) override;

        [[nodiscard]] virtual const BufferLayout &GetLayout() const override { return m_Layout; }
        virtual void SetLayout(const BufferLayout &layout) override { m_Layout = layout; }

    private:
        struct VertexBufferDeleter {
            void operator()(uint32_t ptr) const;
        };

        unique_owner_default<uint32_t, VertexBufferDeleter, 0> m_RendererID;
        BufferLayout m_Layout;
    };

    export class OpenGLIndexBuffer : public IndexBuffer {
    public:
        OpenGLIndexBuffer(uint32_t *indices, uint32_t count);

        virtual ~OpenGLIndexBuffer() override = default;

        virtual void Bind() const override;

        virtual void Unbind() const override;

        [[nodiscard]] virtual uint32_t GetCount() const override { return m_Count; }

    private:
        struct IndexBufferDeleter {
            void operator()(uint32_t ptr) const;
        };

        unique_owner_default<uint32_t, IndexBufferDeleter, 0> m_RendererID;
        uint32_t m_Count;
    };
}
