export module Easy.Renderer.UniformBuffer;

import Easy.Core.Basic;

namespace Easy {

    export class UniformBuffer
    {
    public:
        virtual ~UniformBuffer() {}
        virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

        static Arc<UniformBuffer> Create(uint32_t size, uint32_t binding);
    };

}
