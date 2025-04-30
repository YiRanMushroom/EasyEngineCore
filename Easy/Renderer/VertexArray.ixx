export module Easy.Renderer.VertexArray;

import Easy.Core.Basic;
import Easy.Renderer.Buffer;

namespace Easy {
    export class VertexArray {
    public:
        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;

        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(const Arc<VertexBuffer> &vertexBuffer) = 0;

        virtual void SetIndexBuffer(const Arc<IndexBuffer> &indexBuffer) = 0;

        virtual const std::vector<Arc<VertexBuffer>> &GetVertexBuffers() const = 0;

        virtual const Arc<IndexBuffer> &GetIndexBuffer() const = 0;

        static Arc<VertexArray> Create();
    };
}
