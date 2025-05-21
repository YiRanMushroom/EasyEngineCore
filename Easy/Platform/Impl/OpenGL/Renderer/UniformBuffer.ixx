export module Easy.Platform.Impl.OpenGL.Renderer.UniformBuffer;

import Easy.Core.Basic;
import Easy.Core.Util;
import Easy.Renderer.UniformBuffer;

namespace Easy {
    export class OpenGLUniformBuffer : public UniformBuffer {
    public:
        OpenGLUniformBuffer(uint32_t size, uint32_t binding);

        virtual ~OpenGLUniformBuffer() = default;

        virtual void SetData(const void *data, uint32_t size, uint32_t offset = 0) override;

    private:
        struct UniformBufferDeleter {
            void operator()(uint32_t buffer) const;
        };

        unique_owner_default<uint32_t, UniformBufferDeleter, 0> m_RendererID;
    };
}
